#include "checker.h"
extern int brick_xnum;
extern int brick_ynum;
extern int border_px;
extern int pix_per_brick;
//extern QList<unit*> allunits;
//extern QList<unit*> tempunits;
//extern QList<unit*> errorunits;
//extern QList<unit*> passunits;
checker::checker(QList<unit*> &allunits, QList<unit*> &tempunits, QList<unit*> &errorunits, QList<unit*> &passunits)
{
    tempunits.clear();
    passunits.clear();
    errorunits.clear();
    //Create Checker_map
    checker_map = new int*[brick_xnum];
    for(int i = 0; i < brick_xnum; ++i){
        checker_map[i] = new int[brick_ynum];
        for(int j = 0; j < brick_ynum; j++)
            checker_map[i][j] = 0;
    }

    int border_brick = border_px / pix_per_brick;
    int fill_x, fill_y;

        //Fill Checker_map
    for(unit *item : allunits){
        item->error = false;
        if(item->type == "controlpad")
            continue;
        bool jump = false;
        for(int x=0; x<item->length; x++){
            for(int y=0; y<item->width; y++){
                fill_x = item->xi + x - border_brick;
                fill_y = item->yi + y - border_brick;
                if(fill_x >=0 && fill_y >=0 && fill_x < brick_xnum && fill_y < brick_ynum){
                    if(checker_map[fill_x][fill_y] == 0){
                        checker_map[fill_x][fill_y] = 1;
                    }
                    else{
                        item->error = true;
                        errorunits << item;
                        jump = true;
                        continue;
                    }
                }
            }
            if(jump)
                continue;
        }
        if(jump)
            continue;
        else
            tempunits << item;
    }

        //Check every item. Make sure they all have neighbors
    for(unit *item : tempunits){
        int cross = 0;
        int start_x = item->xi - border_brick;
        int start_y = item->yi - border_brick;


        //Check if it is out of range
        if(start_x < 0 || start_y < 0 || start_x + item->length > brick_xnum || start_y + item->width > brick_ynum){
            item->error = true;
            errorunits << item;
            continue;
        }

        //left
        start_x = item->xi - border_brick - 2;
        if(start_x >= 0){
            for(int a=0; a<item->width; a++){
                if(start_y + a < brick_ynum){
                    if(checker_map[start_x][start_y + a])
                        cross++;
                }
            }
        }
        //right
        start_x = item->xi - border_brick + item->length + 2;
        if(start_x < brick_xnum){
            for(int a=0; a<item->width; a++){
                if(start_y + a < brick_ynum){
                    if(checker_map[start_x][start_y + a])
                        cross++;
                }
            }
        }
        //top
        start_x = item->xi - border_brick;
        start_y = item->yi - border_brick - 2;
        if(start_y >=0){
            for(int a=0; a<item->length; a++){
                if(start_x + a < brick_xnum){
                    if(checker_map[start_x + a][start_y])
                        cross++;
                }
            }
        }
        //down
        start_y = item->yi - border_brick + item->width + 2;
        if(start_y < brick_ynum){
            for(int a=0; a<item->length; a++){
                if(start_x + a < brick_xnum){
                    if(checker_map[start_x + a][start_y])
                        cross++;
                }
            }
        }

        if(cross >= 5){
            passunits << item;
        }
        else{
            errorunits << item;
            item->error = true;
        }
    }
}

//command-line 2048 game

#include<iostream>
#include<string>
#include<cstdlib>               //rand(), srand()
#include<vector>
#include<ctime>                 //time

#define GRID_SIZE    4

using namespace std;

class game{
    int grid[GRID_SIZE][GRID_SIZE];     //2D array to store current grid
    int prev_grid[GRID_SIZE][GRID_SIZE];//2D array to store previous grid (used to detect tile movement)
    int score;                          //current score
    int score_to_add;                   //score to be added after one movement
    bool game_over;                     //true => game over
    char key_pressed;                   //key entered by user
    bool key_accepted;                  //true => key is one of the acceptable choices
    bool new_tile;                      //true => a new tile should be added to the current grid

    public:
    void game_core();                   //main game logic
    void init_grid();                   //initialise grid (from file or default)
    void clear();                       //clear grid
    void option();                      //change grid depend on key_pressed
    void generate_random_tile(int number_of_tiles);//add random tiles
    void check_status();                //check if game is over (no moves available AND full grid)
    bool grid_changed();                //check if grid has changed after move (by comparing to prev_grid)
    void save_prev_grid();              //copy grid to prev_grid
    void display_game();                //display current grid
    void display_end_screen();          //display message when game over
};

//Main function
int main(){
    game main_game;
    main_game.game_core();
    return 0;
}

//Functions of the class game
void game::game_core(){
    //main game
    srand(time(NULL));
    init_grid();
    display_game();
    while(!game_over){
        option();
        if(grid_changed()){
            score+=score_to_add;
            generate_random_tile(1);
            display_game();
        }
        save_prev_grid();
        check_status();
    }

    //game over
    display_end_screen();
}

void game::init_grid(){
    //initialise parameters
    score=0;
    score_to_add=0;
    game_over=false;

    //initialise grid
    clear();
    generate_random_tile(2);

    //initialise prev_grid
    save_prev_grid();
}

void game::clear(){
    for(int i=0;i<GRID_SIZE;i++){
        for(int j=0;j<GRID_SIZE;j++){
            grid[i][j]=0;
        }
    }
}
void game::option(){
    new_tile=true;
    key_accepted=false;

    while(!key_accepted){                               //check if key_pressed is acceptable
        cin>>key_pressed;
        if(key_pressed=='w'||key_pressed=='s'||key_pressed=='a'||key_pressed=='d'){
            key_accepted=true;
        }
    }

    vector<int> tmp;
    int i,j,k;
    if(key_pressed=='w'){                               //UP
        for(j=0;j<GRID_SIZE;j++){                       //remove zeros in one column
            for(i=0;i<GRID_SIZE;i++){
                if(grid[i][j]){
                    tmp.push_back(grid[i][j]);          //store non-zero tile in a vector with variable size
                }
                grid[i][j]=0;                           //clear the column
            }
            for(i=0;i<tmp.size();i++){                  //put non-zero tiles back into grid
                grid[i][j]=tmp[i];
            }
            for(i=0;i<GRID_SIZE-1;i++){                 //check for tiles that can be combined
                if(grid[i][j]==grid[i+1][j]&&grid[i][j]){
                    grid[i][j]*=2;                      //combine tiles if possible
                    score_to_add=grid[i][j];
                    for(k=i+1;k<GRID_SIZE-1;k++){
                        grid[k][j]=grid[k+1][j];        //move the tiles below upwards
                    }
                    grid[GRID_SIZE-1][j]=0;
                }
            }
            tmp.clear();                                //clear tmp for next use
        }
    }
    else if(key_pressed=='s'){                          //DOWN
        for(j=0;j<GRID_SIZE;j++){
            for(i=GRID_SIZE-1;i>=0;i--){
                if(grid[i][j]){
                    tmp.push_back(grid[i][j]);
                }
                grid[i][j]=0;
            }
            for(i=0;i<tmp.size();i++){
                grid[GRID_SIZE-1-i][j]=tmp[i];
            }
            for(i=GRID_SIZE-1;i>0;i--){
                if(grid[i][j]==grid[i-1][j]&&grid[i][j]){
                    grid[i][j]*=2;
                    score_to_add=grid[i][j];
                    for(k=i-1;k>0;k--){
                        grid[k][j]=grid[k-1][j];
                    }
                    grid[0][j]=0;
                }
            }
            tmp.clear();
        }
    }
    else if(key_pressed=='a'){                          //LEFT
        for(i=0;i<GRID_SIZE;i++){
            for(j=0;j<GRID_SIZE;j++){
                if(grid[i][j]){
                    tmp.push_back(grid[i][j]);
                }
                grid[i][j]=0;
            }
            for(j=0;j<tmp.size();j++){
                grid[i][j]=tmp[j];
            }
            for(j=0;j<GRID_SIZE-1;j++){
                if(grid[i][j]==grid[i][j+1]&&grid[i][j]){
                    grid[i][j]*=2;
                    score_to_add=grid[i][j];
                    for(k=j+1;k<GRID_SIZE-1;k++){
                        grid[i][k]=grid[i][k+1];
                    }
                    grid[i][GRID_SIZE-1]=0;
                }
            }
            tmp.clear();
        }
    }
    else if(key_pressed=='d'){                          //RIGHT
        for(i=0;i<GRID_SIZE;i++){
            for(j=GRID_SIZE-1;j>=0;j--){
                if(grid[i][j]){
                    tmp.push_back(grid[i][j]);
                }
                grid[i][j]=0;
            }
            for(j=0;j<tmp.size();j++){
                grid[i][GRID_SIZE-1-j]=tmp[j];
            }
            for(j=GRID_SIZE-1;j>0;j--){
                if(grid[i][j]==grid[i][j-1]&&grid[i][j]){
                    grid[i][j]*=2;
                    score_to_add=grid[i][j];
                    for(k=j-1;k>0;k--){
                        grid[i][k]=grid[i][k-1];
                    }
                    grid[i][0]=0;
                }
            }
            tmp.clear();
        }
    }
}

void game::generate_random_tile(int number_of_tiles){
    int i,j;
    for(int count=0;count<number_of_tiles;count++){
        bool found=false;
        while(!found){
            i=rand()%GRID_SIZE;
            j=rand()%GRID_SIZE;
            if(!grid[i][j]){
                grid[i][j] = rand()%100 > 90 ? 4 : 2;
                found=true;
            }
        }
    }
}

void game::check_status(){
    //check if grid is full
    bool full_grid=true;
    int i,j;
    for(i=0;i<GRID_SIZE&&full_grid;i++){
        for(j=0;j<GRID_SIZE&&full_grid;j++){
            if(!grid[i][j]){
                full_grid=false;
            }
        }
    }

    //check if there are no more moves available
    bool no_more_moves=true;
    for(i=0;i<GRID_SIZE&&no_more_moves;i++){
        for(j=0;j<GRID_SIZE-1&&no_more_moves;j++){
            if((grid[i][j]&&grid[i][j]==grid[i][j+1])||(grid[j][i]&&grid[j][i]==grid[j+1][i])){
                no_more_moves=false;
            }
        }
    }
    
    if(no_more_moves&&full_grid) game_over=true;
}

bool game::grid_changed(){
    int i,j;
    bool same=true;
    for(i=0;i<GRID_SIZE&&same;i++){
        for(j=0;j<GRID_SIZE&&same;j++){
            if(prev_grid[i][j]!=grid[i][j]){
                same=false;
            }
        }
    }
    return !same;
    //NOTE:
    //moves will be executed even if there is no change compared to the previous grid
    //this is so that the grid can be checked against prev_grid to see if grid has changed
    //grid will be displayed only if it has changed (different from prev_grid)
}

void game::save_prev_grid(){
    int i,j;
    for(i=0;i<GRID_SIZE;i++){
        for(j=0;j<GRID_SIZE;j++){
            prev_grid[i][j]=grid[i][j];
        }
    }
}

void game::display_game(){
    system("cls");
    for(int i=0;i<GRID_SIZE-1;i++){
        for(int j=0;j<GRID_SIZE;j++){
            cout<<grid[i][j]<<"\t|";
        }
        cout<<endl<<endl;
    }
    for(int j=0;j<GRID_SIZE;j++){
        cout<<grid[GRID_SIZE-1][j]<<"\t|";
    }
    cout<<"\tScore: "<<score<<endl;
}

void game::display_end_screen(){
    system("cls");
    cout<<"\nGame Over"<<endl;
    cout<<"\nScore: "<<score<<endl;
    cout<<"\nThank you for playing!\n"<<endl;
}

//Author Michelle Smith
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>


#define BOARD_SIZE 7
#define REQ_DISTANCE 3

enum SlotType
{
    Ground,
    Hill,
    City
};

int noOfPlayers;
int totalNoOfPlayersActive;
int row, column;
bool turnCompleted=false;
struct slot* currSlot = NULL;
struct slot *foundSlots;
bool explored[BOARD_SIZE][BOARD_SIZE];
int count =0;

//pointer to slot (0,0)
struct slot *upLeft;

//pointer to slot (0,boardSize -1)
struct slot *upRight;

//pointer to slot (boardSize - 1, 0)
struct slot *downLeft;

//pointer to slot (boardSize - 1, boardSize -1)
struct slot *downRight;


/*
 * The slot template
 */
struct slot{
    //row number
    int row;
    //column number
    int column;
    
    // adjacent left slot
    struct slot *left;
    // adjacent right slot
    struct slot *right;
    // adjacent up slot
    struct slot *up;
    // adjacent down slot
    struct slot *down;
    enum SlotType slotType;
    int slotTypeNo;
    int playerNoPresent[6];
    int noOfPlayersPresentInTheSlot;
}slot;
//Struct to store the information of the player
typedef struct
{
    char name[20];
    char type[20];
    int lifePoints;
    int smartness,strength, magicSkills, luck,dexterity;
    struct slot playerPosition;
    bool isPlaying;
    
}player;

bool gameIsOn=true;
struct slot ** board;
//array to store the player types
char playerTypes[4][20]={"Elf","Human","Ogre","Wizard"};

//array to store the slot types
char slotType[3][20]={"Level Ground","Hill","City"};

//method to set the capabilities of a player
void setCapabilities(player playerArray[],int playerNumber,int playerTypeChoice);

//method to update the capabilities of the player on moving to next slot
void updateCapabiltitesOnMovingSlot(player playerArray[],int playerNumber,int slotTypeMovedTo);

//method to print the details after a successful turn
void printDetails();

//method to find the closest player to attack
int findClosestPlayer(int attackerPlayerNo,struct slot slotArray[], int noOfSlots);

//method to implement the attack
void implementingAttack(player playerArray[],int attackerPlayerNo,int attackedPlayerNo);

//method to perform the actions
void performActions(player playerArray[],struct slot slotArray[],int noOfPlayers,int noOfSlots);

void checkAndUpdateStatusOfPlayer(player playerArray[],int playerNumber);

/*
 * FUNCTIONS PROTOTYPES
 */

/*
 * Functions getBoardSize and getDesiredElement
 * manage the interaction with the users
 */

//Asks the user to provide as input the size of the board
//Returns the size of the board
int getBoardSize();

//Asks the user to insert the row and the column of the element
//she wants to find given a board of size equal to maxsize
void getDesiredElement(int maxsize, int * row, int * col);

/*
 * Functions createBoard and reachDesiredElement
 * manage the creation and manipulation of the board
 */
/*
 * This function creates the board
 * Parameters:
 * 	boardSize: the size of the board
 * 	upLeft: pointer of pointer to slot at position (0, 0)
 * 	upRight: pointer of pointer to slot at position (0, size -1)
 * 	downLeft: pointer of pointer to slot at position (size -1, 0)
 * 	upLeft: pointer of pointer to slot at position (size - 1, size -1)
 */
void createBoard(int boardSize, struct slot **upLeft, struct slot **upRight, struct slot **downLeft, struct slot **downRight);

/*
 * This function traverses the board to find a slot at a predefined
 * position (row, column)
 * Parameters:
 * 	row: the row in which the desired slot is located
 * 	column: the column in which the desired slot is located
 * 	initialSlot: the slot from which the slot search should start
 */
struct slot *  reachDesiredElement(int row, int column, struct slot * initialSlot);


/*
 * The recursive function that traverses the board to find the slots at a predefined
 * distance from the current slot and place them in foundSlots.
 * Parameters:
 * 	reqDist: the required distance from the starting slot
 * 	currDist: the distance of the current slot from the starting slot
 * 	currSlot: a pointer to the current slot that is traversed
 * 	foundSlots: the array of slots that are at a required distance from the starting slot
 * 	count: pointer to an integer representing the number of slots that are found to be at a required distance from the starting slot
 * 	explored: matrix indicating for each slot at row x and column y has been traversed (true) or not (false)
 */
void findSlots(int reqDist, int currDist,  struct slot * currSlot, struct slot * foundSlots, int * count,  bool explored[7][7]);

void playGame(player playerArray[],int playerNumber);
void attack(player playerArray[],int playerNumber);
void moveToAdjacentSlot(player playerArray[],int playerNumber);
int main()
{
    //input the number of players
    printf("Enter the number of players: ");
    
    scanf("%d",&noOfPlayers);
    totalNoOfPlayersActive=noOfPlayers;
    //make a player array to store the player information
    player playerArray[noOfPlayers];
    
    //loop to store the player information
    for(int i=0;i<noOfPlayers;i++)
    {
        //input the player name
        printf("Please enter the name of Player %d :",(i+1));
        scanf("%s",playerArray[i].name);
        
        //input the player type
        printf("Please enter the type of Player %d\n",(i+1));
        printf("Enter 1 for Elf\nEnter 2 for Human\nEnter 3 for Orge\nEnter 4 for Wizard\n");
        int playerTypeChoice=0;
        scanf("%d",&playerTypeChoice);
        
        //loop to check if input is correct
        //if not again asks for input
        while(!(playerTypeChoice>=1 && playerTypeChoice<=4))
        {
            printf("Wrong Choice Entered\nRe-enter your choice: ");
            scanf("%d",&playerTypeChoice);
            
        }
        
        strcpy(playerArray[i].type,playerTypes[playerTypeChoice-1]);
        
        playerArray[i].isPlaying=true;
        
        //calling the method to set the capability of the player
        setCapabilities(playerArray, i,playerTypeChoice);
    }
    
    
    //Creates the board
    createBoard(BOARD_SIZE,&upLeft, &upRight, &downLeft, &downRight);

    srand((unsigned int)(time(NULL)));
    //stores the playerNumber
    for(int i=0;i<noOfPlayers;i++)
    {
        int randomNumber1=rand()%BOARD_SIZE;
        int randomNumber2=rand()%BOARD_SIZE;
        struct slot playerPositionOnBoard;
        playerPositionOnBoard.row=randomNumber1;
        playerPositionOnBoard.column=randomNumber2;
        playerArray[i].playerPosition=playerPositionOnBoard;
        board[randomNumber1][randomNumber2].playerNoPresent[board[randomNumber1][randomNumber2].noOfPlayersPresentInTheSlot]=i;
        board[randomNumber1][randomNumber2].noOfPlayersPresentInTheSlot++;
    }
    
//    for(int i=0;i<BOARD_SIZE;i++)
//    {
//        for(int j=0;j<BOARD_SIZE;j++)
//        {
//            printf("row %d column %d slotType %d Players %d\n",board[i][j].row,board[i][j].column,board[i][j].slotTypeNo,board[i][j].noOfPlayersPresentInTheSlot);
//        }
//    }
    int roundNo=1;
    bool roundIsFinished=false;
    while(gameIsOn)
    {
        printf("*********************************\n");
        printf("          ROUND %d\n",(roundNo));
        printf("*********************************\n");
        roundNo++;
        roundIsFinished=false;
        for(int i=0;i<noOfPlayers;i++)
        {
            if(playerArray[i].isPlaying)
                {
                    playGame(playerArray,i);
                    while(!turnCompleted)
                    {
                        playGame(playerArray,i);
                    }
                    checkAndUpdateStatusOfPlayer(playerArray,i);
                }
                printf("*********************************\n");
            
        }
        if(totalNoOfPlayersActive<=1)
        {
            gameIsOn=false;
        }
    }
    printf("*********************************\n");
    printf("       RESULTS OF THE GAME\n");
    printf("*********************************\n");
    if(totalNoOfPlayersActive==0)
    {
        printf("All Player quit the game, so no winner!\n");
    }
    else
    {
        for(int i=0;i<noOfPlayers;i++)
        {
            if(playerArray[i].isPlaying)
            {
                printf("Winner is %s!\n",playerArray[i].name);
            }
        }
    }
    printf("*********************************\n");
}
void implementDistantAttack(player playerArray[],int attackerPlayerNo, int attackedPlayerNo)
{
    if(!(playerArray[attackedPlayerNo].dexterity>=playerArray[attackerPlayerNo].dexterity))
    {
        playerArray[attackedPlayerNo].lifePoints=playerArray[attackedPlayerNo].lifePoints-(0.3*playerArray[attackerPlayerNo].strength);
    }
}
void playGame(player playerArray[],int playerNumber)
{
    printf("%s : Please do your turn\n",playerArray[playerNumber].name);
    printf("Enter 1 to move to the adjacent slot\nEnter 2 to attack\nEnter 3 to quit the game\n");
    int choice;
    scanf("%d",&choice);
    while(!(choice>=1 && choice<=3))
    {
        printf("Wrong Choice Entered\nRe-enter your choice: ");
        scanf("%d",&choice);
    }
    switch(choice)
    {
        case 1: moveToAdjacentSlot(playerArray, playerNumber);
            turnCompleted=true;
            break;
        case 2: attack(playerArray,playerNumber);
            break;
        case 3: printf("%s : quit the game\n",playerArray[playerNumber].name);
            playerArray[playerNumber].isPlaying=false;
            turnCompleted=true;
            totalNoOfPlayersActive--;
            break;
    }
    printDetails(playerArray);

}
void checkAndUpdateStatusOfPlayer(player playerArray[],int playerNumber)
{
    if(playerArray[playerNumber].lifePoints<=0)
    {
        playerArray[playerNumber].isPlaying=false;
        totalNoOfPlayersActive--;
    }
}
void attack(player playerArray[],int playerNumber)
{
    bool attackCompleted=false;
    turnCompleted=false;
    do{
        struct slot* currSlot = NULL;
        struct slot *foundSlots;
        bool explored[BOARD_SIZE][BOARD_SIZE];
        int count=0;
        for(int i=0; i<BOARD_SIZE; i++){
            for(int j=0; j<BOARD_SIZE;j++){
                explored[i][j] = false;
            }
        }
        printf("Enter 1 to perform a near attack\nEnter 2 to perform a distant attack\nEnter 3 to perform a magic attack.\nEnter 4 to take another option.\n");
        int attackType;
        printf("Attack Type: ");
        scanf("%d",&attackType);
        struct slot presentSlot=board[playerArray[playerNumber].playerPosition.row][playerArray[playerNumber].playerPosition.column];
        int playersThatCanBeAttacked[6][4];
        for(int i=0;i<6;i++)
        {
            playersThatCanBeAttacked[i][0]=-1;
            playersThatCanBeAttacked[i][1]=-1;
            playersThatCanBeAttacked[i][2]=-1;
            playersThatCanBeAttacked[i][3]=-1;
        }
        int counter2=0;
        while(!(attackType>=1 && attackType<=4))
        {
            printf("Wrong choice, Please re-enter your choice.\n");
            printf("Attack Type: ");
            scanf("%d",&attackType);
        }
        switch(attackType)
        {
            case 1:
                if(presentSlot.noOfPlayersPresentInTheSlot>1)
                {
                    for(int i=0;i<presentSlot.noOfPlayersPresentInTheSlot;i++)
                    {
                        if(presentSlot.playerNoPresent[i]!=playerNumber)
                        {
                            printf("Press %d to attack %s in slot(%d, %d).",(counter2+1),playerArray[presentSlot.playerNoPresent[i]].name,presentSlot.row,presentSlot.column);
                            playersThatCanBeAttacked[counter2][0]=counter2+1;
                            playersThatCanBeAttacked[counter2][1]=presentSlot.playerNoPresent[i];
                            playersThatCanBeAttacked[counter2][2]=presentSlot.row;
                            playersThatCanBeAttacked[counter2][3]=presentSlot.column;
                            counter2++;
                        }
                    }
                }
                if(presentSlot.right!=NULL){
                if(presentSlot.right->noOfPlayersPresentInTheSlot>0)
                {
                    for(int i=0;i<presentSlot.right->noOfPlayersPresentInTheSlot;i++)
                    {
                        printf("Press %d to attack %s in slot(%d, %d).",(counter2+1),playerArray[presentSlot.right->playerNoPresent[i]].name,presentSlot.right->row,presentSlot.right->column);
                            playersThatCanBeAttacked[counter2][0]=counter2+1;
                            playersThatCanBeAttacked[counter2][1]=presentSlot.right->playerNoPresent[i];
                            playersThatCanBeAttacked[counter2][2]=presentSlot.right->row;
                        playersThatCanBeAttacked[counter2][3]=presentSlot.right->column;
                        counter2++;
                    }

                }}
                if(presentSlot.left!=NULL){
                if(presentSlot.left->noOfPlayersPresentInTheSlot>0)
                {
                    for(int i=0;i<presentSlot.left->noOfPlayersPresentInTheSlot;i++)
                    {
                        printf("Press %d to attack %s in slot(%d, %d).",(counter2+1),playerArray[presentSlot.left->playerNoPresent[i]].name,presentSlot.left->row,presentSlot.left->column);
                        playersThatCanBeAttacked[counter2][0]=counter2+1;
                        playersThatCanBeAttacked[counter2][1]=presentSlot.left->playerNoPresent[i];
                        playersThatCanBeAttacked[counter2][2]=presentSlot.left->row;
                        playersThatCanBeAttacked[counter2][3]=presentSlot.left->column;
                        counter2++;
                    }
                    
                }}
                if(presentSlot.up!=NULL){
                if(presentSlot.up->noOfPlayersPresentInTheSlot>0)
                {
                    for(int i=0;i<presentSlot.up->noOfPlayersPresentInTheSlot;i++)
                    {
                        printf("Press %d to attack %s in slot(%d, %d).",(counter2+1),playerArray[presentSlot.up->playerNoPresent[i]].name,presentSlot.up->row,presentSlot.up->column);
                        playersThatCanBeAttacked[counter2][0]=counter2+1;
                        playersThatCanBeAttacked[counter2][1]=presentSlot.up->playerNoPresent[i];
                        playersThatCanBeAttacked[counter2][2]=presentSlot.up->row;
                        playersThatCanBeAttacked[counter2][3]=presentSlot.up->column;
                        counter2++;
                    }
                    
                }}
                if(presentSlot.down!=NULL){
                if(presentSlot.down->noOfPlayersPresentInTheSlot>0)
                {
                    for(int i=0;i<presentSlot.down->noOfPlayersPresentInTheSlot;i++)
                    {
                        printf("Press %d to attack %s in slot(%d, %d).",(counter2+1),playerArray[presentSlot.down->playerNoPresent[i]].name,presentSlot.down->row,presentSlot.down->column);
                        playersThatCanBeAttacked[counter2][0]=counter2+1;
                        playersThatCanBeAttacked[counter2][1]=presentSlot.down->playerNoPresent[i];
                        playersThatCanBeAttacked[counter2][2]=presentSlot.down->row;
                        playersThatCanBeAttacked[counter2][3]=presentSlot.down->column;
                        counter2++;
                    }
                    
                }}
                if(counter2>0)
                {
                    int choice;
                    printf("Please enter your choice:");
                    scanf("%d",&choice);
                    while(!(choice>=1 && choice<=counter2))
                    {
                        printf("Wrong choice entered, Please re-enter your choice:");
                        scanf("%d",&choice);
                    }
                    for(int i=0;i<counter2;i++)
                    {
                        if(playersThatCanBeAttacked[i][0]==choice)
                        {
                            implementingAttack(playerArray, playerNumber, playersThatCanBeAttacked[i][1]);
                            attackCompleted=true;
                            turnCompleted=true;
                            printf("Player %s was attacked!\n",playerArray[playersThatCanBeAttacked[i][1]].name);
                        }
                    }
                }
                else{
                    printf("No player found in the same slot or any of the adjacent slots, please try another type of attack\n");
                }
                break;
            case 2:row=presentSlot.row;
                column=presentSlot.column;
                
                //Finds the slot
                
                
                /*If the the required slot is closer to the down-right
                 * corner of the board the search starts from downRight,
                 * which points to slot at position (boardSize-1, boarSize -1)*/
                if(row >= BOARD_SIZE/2){
                    if(column >= BOARD_SIZE/2)
                        currSlot = reachDesiredElement(row,column,downRight);
                    else
                    /*If the the required slot is closer to the down-left
                     * corner of the board the search starts from downLeft,
                     * which points to slot at position (boardSize-1, 0)*/
                        currSlot = reachDesiredElement(row,column,downLeft);
                } else {
                    /*If the the required slot is closer to the up-right
                     * corner of the board the search starts from upRight,
                     * which points to slot at position (0, boarSize -1)*/
                    if(column >= BOARD_SIZE/2)
                        currSlot = reachDesiredElement(row,column, upRight);
                    /*If the the required slot is closer to the up-left
                     * corner of the board the search starts from upLeft,
                     * which points to slot at position (0, 0)*/
                    else currSlot = reachDesiredElement(row,column,upLeft);
                }
                
                for(int i=0; i<BOARD_SIZE; i++){
                    for(int j=0; j<BOARD_SIZE;j++){
                        explored[i][j] = false;
                    }
                }
                int foundSlotsToAttack[49][3];
                for(int i=0;i<49;i++)
                {
                    foundSlotsToAttack[i][0]=-1;
                    foundSlotsToAttack[i][1]=-1;
                    foundSlotsToAttack[i][2]=-1;
                }
                int counter3=0;
                foundSlots = malloc(BOARD_SIZE * BOARD_SIZE * sizeof(struct slot ));
                //printf("\n\nFunction findSlotsinvoked:\n");
                for(int distance=2;distance<=4;distance++)
                {
                    //printf("Distance %d\n",distance);
                    if(currSlot!= NULL){
                        //invokes function findSlots. The required distance is a constant
                        //However you can reuse this function providing as input any arbitrary distance
                        findSlots(distance, 0, currSlot, foundSlots, &count, explored);
                        for(int i=0; i<count; i++){
                            //printf("(%d, %d)-> ",foundSlots[i].row, foundSlots[i].column);
                            foundSlotsToAttack[counter3][0]=foundSlots[i].row;
                            foundSlotsToAttack[counter3][1]=foundSlots[i].column;
                            foundSlotsToAttack[counter3][1]=distance;
                            counter3++;
                        }
                    }

                }
                int counter4=0;
                int playerOptionsToAttack[6][2];
                for(int i=0;i<6;i++)
                {
                    playerOptionsToAttack[i][0]=-1;
                    playerOptionsToAttack[i][1]=-1;
                }
                for(int i=0;i<noOfPlayers;i++)
                {
                    if(playerArray[i].isPlaying && i!=playerNumber)
                    {
                        for(int j=0;j<counter3;j++)
                        {
                            if((playerArray[i].playerPosition.row== foundSlotsToAttack[j][0])&&(playerArray[i].playerPosition.column== foundSlotsToAttack[j][1]))
                               {
                                   printf("Press %d to attack Player %s at slot(%d, %d) at a distance %d\n",(counter4+1),playerArray[i].name,playerArray[i].playerPosition.row,playerArray[i].playerPosition.column,foundSlotsToAttack[j][1]);
                                   playerOptionsToAttack[counter4][0]=i;
                                   playerOptionsToAttack[counter4][1]=counter4+1;
                                   counter4=counter4+1;
                                   break;
                               }
                        }
                    }
                }
                if(counter4>0){
                int playerToAttackChoice;
                printf("Please enter your choice:");
                scanf("%d",&playerToAttackChoice);
                while(!(playerToAttackChoice>=1 && playerToAttackChoice<=counter4))
                {
                    printf("Wrong choice entered. Please re-enter your choice\n");
                    scanf("%d",&playerToAttackChoice);
                }
                for(int i=0;i<counter4;i++)
                {
                    if(playerToAttackChoice==playerOptionsToAttack[i][1])
                    {
                        implementingAttack(playerArray, playerNumber, playerOptionsToAttack[i][0]);
                        break;
                    }
                }
                    attackCompleted=true;
                    turnCompleted=true;
                }
                else{
                    printf("No player found in the vicinity!\n");
                }
                break;
            case 3:
                if(playerArray[playerNumber].smartness+playerArray[playerNumber].magicSkills>150)
                {
                    printf("**MAGIC ATTACK**\nPlease choose the player to attack\n");
                    int playerToAttack[noOfPlayers];
                    int counter=1;
                    for(int i=0;i<noOfPlayers;i++)
                    {
                        playerToAttack[i]=-1;
                        if(playerArray[i].isPlaying){
                            printf("Press %d to attack %s\n",(counter),playerArray[i].name);
                            playerToAttack[i]=counter;
                            counter++;
                        }
                    }
                    printf("Please enter your choice:");
                    int playerChosen;
                    scanf("%d",&playerChosen);
                    while(!(playerChosen>=1 && playerChosen<=counter))
                    {
                        printf("Wrong choice, Please re-enter your choice.\n");
                        printf("Please enter your choice:");
                        scanf("%d",&playerChosen);
                    }
                    for(int i=0;i<noOfPlayers;i++)
                    {
                        if(playerToAttack[i]==playerChosen)
                        {
                            attackCompleted=true;
                            turnCompleted=true;
                            playerArray[i].lifePoints=playerArray[i].lifePoints-((0.5*playerArray[playerNumber].magicSkills)+(0.2*playerArray[playerNumber].smartness));
                            break;
                        }
                    }
                   
                }
                else{
                    printf("Magic Attack cannot be performed as the condition to perform the attack is not fulfilled!\n");
                }
                break;
            case 4: turnCompleted=false;
                attackCompleted=true;
        }
        
    }while(!attackCompleted);
}
void moveToAdjacentSlot(player playerArray[],int playerNumber)
{
    int totalNoOfOptionsToMove=0;
    int slotTypeMovedTo;
    int possiblePositionsToMove[4][2];
    int possibleInputs[4];
    for(int i=0;i<4;i++)
    {
        possiblePositionsToMove[i][0]=-1;
        possiblePositionsToMove[i][1]=-1;
        possibleInputs[i]=-1;
    }
    struct slot presentSlot=board[playerArray[playerNumber].playerPosition.row][playerArray[playerNumber].playerPosition.column];
    printf("The Player has chosen to move to an adjacent slot\n");
    printf("The slots available are:\n");

    if(presentSlot.right!=NULL)
    {
        possiblePositionsToMove[0][0]=presentSlot.right->row;
        possiblePositionsToMove[0][1]=presentSlot.right->column;
        printf("Press %d to move RIGHT to slot(%d,%d).\n",(totalNoOfOptionsToMove+1),possiblePositionsToMove[0][0],possiblePositionsToMove[0][1]);
        possibleInputs[0]=(totalNoOfOptionsToMove+1);
        totalNoOfOptionsToMove++;
    }
    if(presentSlot.left!=NULL)
    {
        possiblePositionsToMove[1][0]=presentSlot.left->row;
        possiblePositionsToMove[1][1]=presentSlot.left->column;
        printf("Press %d to move LEFT to slot(%d,%d).\n",(totalNoOfOptionsToMove+1),possiblePositionsToMove[1][0],possiblePositionsToMove[1][1]);
        possibleInputs[1]=(totalNoOfOptionsToMove+1);
        totalNoOfOptionsToMove++;
    }
    if(presentSlot.up!=NULL)
    {
        possiblePositionsToMove[2][0]=presentSlot.up->row;
        possiblePositionsToMove[2][1]=presentSlot.up->column;
        printf("Press %d to move UP to slot(%d,%d).\n",(totalNoOfOptionsToMove+1),possiblePositionsToMove[2][0],possiblePositionsToMove[2][1]);
        possibleInputs[2]=(totalNoOfOptionsToMove+1);
        totalNoOfOptionsToMove++;
    }
    if(presentSlot.down!=NULL)
    {
        possiblePositionsToMove[3][0]=presentSlot.down->row;
        possiblePositionsToMove[3][1]=presentSlot.down->column;
        printf("Press %d to move DOWN to slot(%d,%d).\n",(totalNoOfOptionsToMove+1),possiblePositionsToMove[3][0],possiblePositionsToMove[3][1]);
        possibleInputs[3]=(totalNoOfOptionsToMove+1);
        totalNoOfOptionsToMove++;
    }
    int choice;
    printf("Choice:");
    scanf("%d",&choice);
    while(!(choice>=1 && choice<=totalNoOfOptionsToMove))
    {
        printf("Wrong choice entered! Please re-enter your choice.\n");
        printf("Choice:");
        scanf("%d",&choice);
    }
    if(possibleInputs[0]==choice)
    {
        printf("Player %s: moves right to slot(%d, %d).",playerArray[playerNumber].name,possiblePositionsToMove[0][0],possiblePositionsToMove[0][1]);
        playerArray[playerNumber].playerPosition.row=possiblePositionsToMove[0][0];
        playerArray[playerNumber].playerPosition.column=possiblePositionsToMove[0][1];
        slotTypeMovedTo=presentSlot.right->slotTypeNo;
    }
    else if(possibleInputs[1]==choice)
    {
        printf("Player %s: moves left to slot(%d, %d).",playerArray[playerNumber].name,possiblePositionsToMove[1][0],possiblePositionsToMove[1][1]);
        playerArray[playerNumber].playerPosition.row=possiblePositionsToMove[1][0];
        playerArray[playerNumber].playerPosition.column=possiblePositionsToMove[1][1];
        slotTypeMovedTo=presentSlot.left->slotTypeNo;
    }
    else if(possibleInputs[2]==choice)
    {
        printf("Player %s: moves up to slot(%d, %d).",playerArray[playerNumber].name,possiblePositionsToMove[2][0],possiblePositionsToMove[2][1]);
        playerArray[playerNumber].playerPosition.row=possiblePositionsToMove[2][0];
        playerArray[playerNumber].playerPosition.column=possiblePositionsToMove[2][1];
        slotTypeMovedTo=presentSlot.up->slotTypeNo;
    }
    else if(possibleInputs[3]==choice)
    {
        printf("Player %s: moves down to slot(%d, %d).",playerArray[playerNumber].name,possiblePositionsToMove[3][0],possiblePositionsToMove[3][1]);
        playerArray[playerNumber].playerPosition.row=possiblePositionsToMove[3][0];
        playerArray[playerNumber].playerPosition.column=possiblePositionsToMove[3][1];
        slotTypeMovedTo=presentSlot.down->slotTypeNo;
    }
    updateCapabiltitesOnMovingSlot(playerArray, playerNumber, slotTypeMovedTo);
    
}
void printDetails(player playerArray[])
{
    for(int i=0;i<noOfPlayers;i++)
    {
        if(playerArray[i].isPlaying){
            printf("%s\t(%s, %d)\n",playerArray[i].name,playerArray[i].type,playerArray[i].lifePoints);
        }
    }
}
void setCapabilities(player playerArray[],int playerNumber,int playerTypeChoice)
{
    playerArray[playerNumber].lifePoints=100;
    srand((unsigned int)time(NULL));
    if(playerTypeChoice==1)
    {
        playerArray[playerNumber].magicSkills=51+(rand()%29);
        playerArray[playerNumber].smartness=70+(rand()%31);
        playerArray[playerNumber].luck=60+(rand()%41);
        playerArray[playerNumber].dexterity=1+(rand()%100);
        playerArray[playerNumber].strength=1+(rand()%50);
        
    }
    else if(playerTypeChoice==2)
    {
        playerArray[playerNumber].magicSkills=1+(rand()%100);
        playerArray[playerNumber].smartness=1+(rand()%100);
        playerArray[playerNumber].luck=1+(rand()%100);
        playerArray[playerNumber].dexterity=1+(rand()%100);
        playerArray[playerNumber].strength=1+(rand()%100);
        while((playerArray[playerNumber].luck+playerArray[playerNumber].magicSkills+playerArray[playerNumber].smartness+playerArray[playerNumber].strength+playerArray[playerNumber].dexterity)>=300)
        {
            playerArray[playerNumber].magicSkills=1+(rand()%100);
            playerArray[playerNumber].smartness=1+(rand()%100);
            playerArray[playerNumber].luck=1+(rand()%100);
            playerArray[playerNumber].dexterity=1+(rand()%100);
            playerArray[playerNumber].strength=1+(rand()%100);
        }
    }
    else if(playerTypeChoice==3)
    {
        playerArray[playerNumber].magicSkills=0;
        playerArray[playerNumber].smartness=rand()%21;
        playerArray[playerNumber].luck=(rand()%(50-playerArray[playerNumber].smartness));
        playerArray[playerNumber].dexterity=80+(rand()%21);
        playerArray[playerNumber].strength=80+(rand()%21);
    }
    else
    {
        playerArray[playerNumber].magicSkills=80+(rand()%21);
        playerArray[playerNumber].smartness=90+(rand()%11);
        playerArray[playerNumber].luck=50+(rand()%51);
        playerArray[playerNumber].dexterity=1+(rand()%100);
        playerArray[playerNumber].strength=1+(rand()%20);
    }
}
void updateCapabiltitesOnMovingSlot(player playerArray[],int playerNumber,int slotTypeMovedTo)
{
    if(slotTypeMovedTo==1)
    {
        if(playerArray[playerNumber].dexterity<50)
        {
            playerArray[playerNumber].strength-=10;
        }
        else if(playerArray[playerNumber].dexterity>=60)
        {
            playerArray[playerNumber].strength+=10;
        }
    }
    else if(slotTypeMovedTo==2)
    {
        if(playerArray[playerNumber].smartness>60)
        {
            playerArray[playerNumber].magicSkills+=10;
        }
        else if(playerArray[playerNumber].smartness<=50)
        {
            playerArray[playerNumber].dexterity-=10;
        }
        
    }
}
void implementingAttack(player playerArray[],int attackerPlayerNo,int attackedPlayerNo)
{
    if(playerArray[attackedPlayerNo].strength<=70)
    {
        playerArray[attackedPlayerNo].lifePoints=playerArray[attackedPlayerNo].lifePoints-(0.5*playerArray[attackedPlayerNo].strength);
    }
    else
    {
        playerArray[attackerPlayerNo].lifePoints=playerArray[attackerPlayerNo].lifePoints-(0.3*playerArray[attackedPlayerNo].strength);
    }
}


/*
 * This function creates the board
 * Parameters:
 * 	size: the size of the board
 * 	upLeft: pointer of pointer to slot at position (0, 0)
 * 	upRight: pointer of pointer to slot at position (0, boardSize -1)
 * 	downLeft: pointer of pointer to slot at position (boardSsize -1, 0)
 * 	upLeft: pointer of pointer to slot at position (boardSize - 1, boardSize -1)
 */
void createBoard(int boardSize, struct slot **upLeft, struct slot **upRight, struct slot **downLeft, struct slot **downRight){
    
    //The board is represented as a pointer of pointer to slots
    //This allocates in memory the space for the pointers to each row of the board
    board = malloc(boardSize * sizeof(struct slot *));
    
    
    for(int i =0; i< boardSize; i++){
        //This allocates in memory the space for the slots in each row of the board
        board[i] = malloc(boardSize * sizeof(struct slot));
        
        //For each slot it sets up the row and column number
        for(int j=0;j < boardSize; j++){
            board[i][j].row = i;
            board[i][j].column = j;
        }
    }
    
    //It sets up the adjacent slots for the slots that are
    //not at the borders. These slots have 4 adjacent elements
    for(int i =1; i< boardSize-1; i++){
        for(int j=1;j < boardSize-1; j++){
            board[i][j].up = &board[i-1][j];
            board[i][j].right = &board[i][j+1];
            board[i][j].down = &board[i+1][j];
            board[i][j].left = &board[i][j-1];
        }
    }
    
    //It sets up the adjacent slots for the slots that are
    //in the first and the last row, except the slots at the edges.
    //
    for(int j = 1; j < boardSize -1; j++){
        //It sets up the adjacent slots for the slots that are in the first row.
        //These slots don't have an adjacent element on top of them
        // because they are on the first row of the board
        board[0][j].right = &board[0][j+1];
        board[0][j].left = &board[0][j-1];
        board[0][j].down = &board[1][j];
        board[0][j].up = NULL;
        
        //It sets up the adjacent slots for the slots that are in the last row.
        //These slots don't have an adjacent element on down them
        // because they are on the last row of the board
        board[boardSize - 1][j].right = &board[boardSize - 1][j+1];
        board[boardSize -1][j].left = &board[boardSize - 1][j-1];
        board[boardSize - 1][j].up = &board[boardSize - 2][j];
        board[boardSize - 1][j].down = NULL;
    }
    
    //It sets up the adjacent slots for the slots that are
    //in the first and the last column, except the slots at the edges.
    //
    for(int i = 1; i < boardSize -1; i++){
        //It sets up the adjacent slots for the slots that are in the first column.
        //These slots don't have an adjacent element on the left
        // because they are on the first column of the board
        board[i][0].right = &board[i][1];
        board[i][0].up = &board[i-1][0];
        board[i][0].down = &board[i+1][0];
        board[i][0].left = NULL;
        
        //It sets up the adjacent slots for the slots that are in the last column.
        //These slots don't have an adjacent element on the right
        // because they are on the last column of the board
        board[i][boardSize-1].left = &board[i][boardSize-2];
        board[i][boardSize -1].up = &board[i -1][boardSize-1];
        board[i][boardSize -1].down = &board[i+1][boardSize -1];
        board[i][boardSize -1].right = NULL;
    }
    
    
    //It sets up the adjacent slots for the slot at position (0,0).
    //This only has only 2 adjacent slots: right and down
    board[0][0].right = &board[0][1];
    board[0][0].down = &board[1][0];
    
    //It sets up the adjacent slots for the slot at position (0,boardSize -1).
    //This only has only 2 adjacent slots: left and down
    board[0][boardSize-1].left = &board[0][boardSize-2];
    board[0][boardSize -1].down = &board[1][boardSize -1];
    
    //It sets up the adjacent slots for the slot at position (boarSize -1 ,0).
    //This only has only 2 adjacent slots: up and right
    board[boardSize -1][0].right = &board[boardSize -1][1];
    board[boardSize -1][0].up = &board[boardSize -2][0];
    
    //It sets up the adjacent slots for the slot at position (boarSize -1 ,boardSize-1).
    //This only has only 2 adjacent slots: up and left
    board[boardSize - 1][boardSize-1].up = &board[boardSize-2][boardSize-1];
    board[boardSize - 1][boardSize -1].left = &board[boardSize -1][boardSize -2];
    
    
    
    
    
    
    
    //assigns a pointer to slot at position (0, 0)
    *upLeft = &board[0][0];
    //assigns pointer of pointer to slot at position (0, boardSize -1)
    *upRight = &board[0][boardSize -1];
    //assigns pointer of pointer to slot at position ( boardSize -1,)
    *downLeft = &board[boardSize -1][0];
    //assigns pointer of pointer to slot at position (boardSize -1, boardSize -1)
    *downRight = &board[boardSize -1][boardSize -1];
    
     srand((unsigned int)time(NULL));
    for(int i=0;i<boardSize;i++)
    {
        for(int j=0;j<boardSize;j++)
        {
            board[i][j].slotTypeNo=rand()%3;
            board[i][j].slotType=board[i][j].slotTypeNo;
            for(int k=0;k<6;k++)
            {
                board[i][j].playerNoPresent[k]=-1;
            }
            board[i][j].noOfPlayersPresentInTheSlot=0;
        }
    }
}



/*
 * This function traverses the board to find a slot at a predefined
 * position (row, column). This function returns a pointer to the desired slot
 * Parameters:
 * 	row: the row in which the desired slot is located
 * 	column: the column in which the desired slot is located
 * 	initialSlot: the slot from which the slot search should start
 */
struct slot * reachDesiredElement(int row, int column, struct slot * initialSlot){
    
    bool found = false;
    //current slot
    struct slot * currentSlot = initialSlot;
    
    //printf("\nFunction reachDesiredElement invoked\n");
    
    //prints the column and the row of the initial slot from which the search starts
    //printf("Initial slot (%d, %d) -> \n",initialSlot->row,initialSlot->column);
    
    
    //while the slot is not found
    while(found == false){
        
        
        //if the row of the current slot is > of the row of the desired slot,
        //we move up
        if(currentSlot->row > row){
            //the current slot now points to the slot one row up
            currentSlot = currentSlot->up;
            //prints the column and the row of the current slot
            //printf("Current slot (%d, %d) -> \n",currentSlot->row,currentSlot->column);
        }
        //if the row of the current slot is < of the row of the desired slot,
        //we move down
        if(currentSlot->row < row){
            //the current slot now points to the slot one row down
            currentSlot = currentSlot->down;
            //prints the row and the column of the current slot
            //printf("Current slot (%d, %d) -> \n",currentSlot->row,currentSlot->column);
            
        }
        //if the column of the current slot is > of the column of the desired slot,
        //we move left
        if(currentSlot->column > column){
            
            //the current slot now points to the slot one column left
            currentSlot = currentSlot->left;
            //prints the row and the column of the current slot
            //printf("Current slot (%d, %d) -> \n",currentSlot->row,currentSlot->column);
        }
        
        //if the column of the current slot is < of the column of the desired slot,
        //we move right
        if(currentSlot->column < column){
            
            //the current slot now points to the slot one column right
            currentSlot = currentSlot->right;
            //prints the row and the column of the current slot
            //printf("Current slot (%d, %d) -> \n",currentSlot->row,currentSlot->column);
            
        }
        //if the current slot is at a column and a row equal to the desired column and row, respectively
        // we found the slot
        if(currentSlot->column == column && currentSlot->row == row){
            printf("Found\n");
            found = true;
            
        }
        
    }
    //returns the found slot
    return currentSlot;
}

/*
 * The recursive function that traverses the board to find the slots at a predefined
 * distance from the current slot and place them in foundSlots.
 * Parameters:
 * 	reqDist: the required distance from the starting slot
 * 	currDist: the distance of the current slot from the starting slot
 * 	currSlot: a pointer to the current slot that is traversed
 * 	foundSlots: the array of slots that are at a required distance from the starting slot
 * 	count: pointer to an integer representing the number of slots that are found to be at a required distance from the starting slot
 * 	explored: matrix indicating for each slot at row x and column y has been traversed (true) or not (false)
 */
void findSlots(int reqDist, int currDist,  struct slot * currSlot, struct slot * foundSlots, int * count,  bool explored[BOARD_SIZE][BOARD_SIZE]){
    
    
    
    //The base case: the current slot is at the required distance from the starting slot
    if(currDist == reqDist){
        //the current slot was not explored
        if(explored[currSlot->row][currSlot->column] == false){
            //The next availbale position (indicated by count) at foundSlots points to the current slot
            *(foundSlots + *count) = *currSlot;
            //the counter is incremented
            (*count)++;
            //the matrix of the explored slots set to true the element at the row and column of the current slot
            explored[currSlot->row][currSlot->column] = true;
        }
    }
    //The recursive call: the current slot is at a distance that is less than the required distance (more slots still have to be traversed)
    else{
        //if the current slot has the up slot != NULL (i.e. the slot is not in the first row)
        if(currSlot->up != NULL){
            //invokes function find slots incrementing the current Distance (currDist) and setting the current slot to the up slot
            findSlots(reqDist, currDist +1,  currSlot->up, foundSlots, count, explored);
        }
        //if the current slot has the right slot != NULL (i.e. the slot is not in the last column)
        if(currSlot->right != NULL){
            //invokes function find slots incrementing the current Distance (currDist) and setting the current slot to the right slot
            findSlots(reqDist, currDist +1,  currSlot->right, foundSlots, count, explored);
        }
        //if the current slot has the down slot != NULL (i.e. the slot is not in the last row)
        if(currSlot->down != NULL){
            //invokes function find slots incrementing the current Distance (currDist) and setting the current slot to the down slot
            findSlots(reqDist, currDist +1,  currSlot->down, foundSlots, count, explored);
        }
        //if the current slot has the left slot != NULL (i.e. the slot is not in the first column)
        if(currSlot->left != NULL){
            //invokes function find slots incrementing the current Distance (currDist) and setting the current slot to the left slot
            findSlots(reqDist, currDist +1,  currSlot->left, foundSlots, count, explored);
        }
        
    }
    
    
}

int getBoardSize(){
    int boardSize;
    printf("Please enter the required size of the board\n");
    //asks the user to enter the desired board size
    scanf("%d", &boardSize);
    return boardSize;
}

//THis function takes as input the size of the board and the pointers
// to the integers representing the row and the column of the slot that
// the user want to retrieve
void getDesiredElement(int boardSize, int * row, int * col){
    printf("Please enter the column and the Row of the desired slot \n");
    printf("Note that size of the board is %d\n", boardSize);
    
    //it will cycle asking the user to insert the row
    //until the value of the desired row is >= 0 or < of the
    // size of the board
    do {
        printf("Row: ");
        scanf("%d", row);
        printf("%d\n", *row);
        if(row < 0 && *row >= boardSize)
            printf("Error: Incorrect row dimension\n");
    } while(*row < 0 && *row >= boardSize);
    
    //it will cycle asking the user to insert the column
    //until the value of the desired row is >= 0 or < of the
    // size of the board
    do {
        printf("Column: ");
        scanf("%d", col);
        printf("%d\n", *col);
        if(*col < 0 && *col >= boardSize)
            printf("Error: Incorrect column dimension\n");
    } while(*col < 0 && *col >= boardSize);
}





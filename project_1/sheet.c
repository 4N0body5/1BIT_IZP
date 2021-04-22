/*
** project 1: Text processing
** author: Natalia Bubakova
** login: xbubak01
** group: tpl 30_Po 15:00 - 16:50, lab. N103, as. Husa
** date: 15 November 2020
** academic year: 2020/2021
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LENGTH 10241
#define CELL_LENGTH 101


int error(int e);                                                                     //errors
void subst_delim (char *line, char *delim);                                           //delimiter
int col_count(char *line, char delim);                                                //columns counter
int irow(int columns,char delim);                                                     //irow R + arow
int dcol(char *line,int column,char delim);                                           //dcol C
int dcols(char *line,int from_column,int to_column,char delim);                       //dcols N M
int icol(char *line,int column,char delim);                                           //icol C
int rows(char *row,char *line,char *N_row,char *M_row,int line_count);                //rows N M
void with_string(char *row,char *line,char *string,int column,char delim,int down_up);//beginswith C STR + contains C STR
int cset(char *line,char *string,int column,char delim);                              //cset C STR
int lower_upper(char *line,int column,char delim,int down_up);                        //tolower C + toupper C
int int_round(char *line,int column,char delim,int down_up);                          //int C + round C
int copy_swap(char *line,int N_column,int M_column,char delim,int down_up);           //copy N M + swap N M
int move(char *line,int N_column,int M_column,char delim);                            //move N M
void N_foo(char *N_cell,char *line,char delim,int N_column);                          //subfunctions to copy_swap and move
void M_foo(char *M_cell,char *line,char delim,int M_column);
int cnum(char *line,int column,int N_column,int M_column,char delim,int click);       //csum C N M + cavg C N M + cmin C N M + cmax C N M
void numbers(double *array,char *line,int N_column,int M_column,char delim);          //subfunction to cnum



int main(int argc, char *argv[])
{

  if (argc == 1)
    return error(1);

  char *commands[32] = {"-d","irow","arow","drow","drows","icol","acol","dcol","dcols","rows","beginswith","contains","cset","tolower","toupper","round","int","copy","swap","move","csum","cavg","cmin","cmax","ccount","cseq","rseq","rsum","ravg","rmin","rmax","rcount"};
  char line[LINE_LENGTH];
  char row[LINE_LENGTH];    //subtool for row selection
  char *delim = " ";
  int line_count = 0;
  int columns;
  int index,loc,e;          //helps to process data commands and detect errors
  int done = 0;             //assures program that command is done and no error occured
  int t = 0;                //enables program to work with more table commands
  int tab_com[100];         //helps to process more table commands at once
  int arow = 0;
  int cols,delcol = 0;      //helps to process correctly all combinations of dcols and irows
  int rs_loc,rs = 0;        //helps to process row selection


  for (int i=1; i<argc; i++)                     //checks all the arguments to make sure no command was forgotten and there is no error
  {
    if (((int)argv[i][0] >= 'a' && (int)argv[i][0] <= 'z') || i==1) //if error, program is spared from all the cycles
    {
      e=0;
      for (int j=0; j<32; j++)                   //goes through the commands
      {
        if (strcmp(argv[i],commands[j]) == 0)   //compares the arguments with commands
        {
          e++;
          switch (j)        //*due to the arguments and effort to limit pointers or errors,*
          {            //*I could not put it into the function (sorry for the enormous length of my main)*
            case 0:                       // "-d"
              i++;
              if (i < argc)
                delim = argv[i];    //"-d" was found, thus uploads delimiter
              else
                return error(3);
            break;
       //
            case 1:                       // "irow"
            case 3:                       // "drow"
            case 5:                       // "icol"
            case 7:                       // "dcol"
              i++;
              if (i < argc &&  atoi(argv[i]) > 0)    //checks data to process command
              {
                tab_com[t]=j;   //collects data to an array to process more table commands at once
                t++;            //an even index induces name of command, the odd one position in argv
                tab_com[t]=i-1;
                t++;
                if (j==7)
                  delcol++;     //prevents an inacuraccy in counting columns when some are deleted
              }
              else
                return error(3);
            break;

            case 2:                       // "arow"
            case 6:                       // "acol"
              tab_com[t]=j;
              t++;
              tab_com[t]=i;
              t++;
              if (j == 2)
                arow++;         //helps to process the command "arow"
            break;

            case 4:                       // "drows"
            case 8:                       // "dcols"
              i++;
              if (i < argc &&  atoi(argv[i]) > 0)
              {
                i++ ;
                if (i < argc &&  atoi(argv[i]) > 0  &&  atoi(argv[i]) >= atoi(argv[i-1])) // N < M
                {
                  tab_com[t]=j;
                  t++;
                  tab_com[t]=i-2;
                  t++;
                  if (j == 8)
                    delcol+=(atoi(argv[i])-atoi(argv[i-1]))+1;
                }
                else
                return error(3);
              }
              else
                return error(3);
            break;
    //
            case 9:                       // "rows"
              if (rs != 0)
               return error(2);
              i++;
              if (i < argc &&  (atoi(argv[i]) > 0 || (int)argv[i][0]=='-' ))
              {
                i++ ;
                if (i < argc &&  ((atoi(argv[i]) > 0  &&  i >= i-1) || (int)argv[i][0]=='-'))
                {
                    rs = j;               //helps to process particular row selections
                    rs_loc = i-2;         //provides information about its position in argv
                }
                else
                return error(3);
              }
              else
                return error(3);
            break;

            case 10:                       // "beginswith"
            case 11:                       // "contains"
              if (rs != 0)
               return error(2);
              i++;
              if (i < argc &&  atoi(argv[i]) > 0)
              {
                i++ ;
                if (i < argc)
                {
                    rs = j;
                    rs_loc = i-2;
                }
                else
                return error(3);
              }
              else
                return error(3);
            break;
    //
            case 12:                       // "cset"
            i++;
            if (i < argc &&  atoi(argv[i]) > 0)
            {
              i++ ;
              if (i < argc)
              {
                index = j;              //helps to process particular data commands
                loc = i-2;           //provides information about its position in argv
              }
              else
              return error(3);
            }
            else
              return error(3);
            break;

            case 13:                       // "tolower"
            case 14:                       // "toupper"
            case 15:                       // "round"
            case 16:                       // "int"
              i++;
              if (i < argc &&  atoi(argv[i]) > 0)
              {
                index = j;
                loc = i-1;
              }
              else
                return error(3);
            break;

            case 17:                       // "copy"
            case 18:                       // "swap"
            case 19:                       // "move"
              i++;
              if (i < argc &&  atoi(argv[i]) > 0)
              {
                i++ ;
                if (i < argc &&  atoi(argv[i]) > 0  &&  atoi(argv[i]) >= atoi(argv[i-1]))
                {
                  index = j;
                  loc = i-2;
                }
                else
                return error(3);
              }
              else
                return error(3);
            break;

            case 20:                       // "csum"
            case 21:                       // "cavg"
            case 22:                       // "cmin"
            case 23:                       // "cmax"
              i++;
              if (i < argc &&  atoi(argv[i]) > 0)
              {
                i++;
                if (i < argc &&  atoi(argv[i]) > 0)
                {
                  i++;
                  if (i < argc &&  atoi(argv[i]) > 0  &&  atoi(argv[i]) >= atoi(argv[i-1]))
                  {
                    if (atoi(argv[i-2]) < atoi(argv[i-1]) || atoi(argv[i-2]) > atoi(argv[i])) // C does not belong to interval <N,M>
                    {
                      index = j;
                      loc = i-3;
                    }
                    else
                      return error(3);
                  }
                  else
                    return error(3);
                }
                else
                return error(3);
              }
              else
                return error(3);
            break;

            default:
              if (j>=24 && j<=31)
                return error(0);
          }//switch
        }//if
      }//for
      if (e==0)
        return error(2);
    }
    else
      return error(2);
  }



  while(fgets(line,LINE_LENGTH,stdin) != NULL)
  {
    line_count++;
    subst_delim(line,delim);        //by delimiter substitution, it prepares line for commands

    if (line_count == 1)
    {
      columns = col_count(line,delim[0]);   //number of columns depends on the first row
         if (delcol > 0)
           cols=columns-delcol;             //change when delcol command occures
     }
    else if (col_count(line,delim[0]) != columns) //error when columns are not uniform
      return error(4);

    if (t > 0)            //processes more table commands at once
    {
      for (int j = 0; j < t; j+=2)
      {
        switch (tab_com[j])
        {
              case 1:                       // "irow"
                 if (line_count == atoi(argv[tab_com[j+1]+1]))
                 {
                   if (delcol > 0)
                   {
                     columns=col_count(line,delim[0])-delcol;
                   }
                   done = irow(cols,delim[0]);    //'done' checks whether command was proceeded
                 }
              break;

              case 2:                       // "arow"
              break;

              case 3:                       // "drow"
              if (line_count == atoi(argv[tab_com[j+1]+1]))
              {
                line[0]='\0';
                done=1;
              }
              break;

              case 4:                       // "drows"
              if (line_count >= atoi(argv[tab_com[j+1]+1]) && line_count <= atoi(argv[tab_com[j+1]+2]))
              {
                line[0]='\0';
                done=1;
              }
              break;

              case 5:                       // "icol"
                done=icol(line,atoi(argv[tab_com[j+1]+1]),delim[0]);
              break;

              case 6:                       // "acol"
              line[strlen(line)-2]=delim[0];  // strcat(line,&delim[0]);
              line[strlen(line)-1]='\n';  // strcat(line,"\n");
                done=1;
              break;

              case 7:                       // "dcol"
                done=dcol(line,atoi(argv[tab_com[j+1]+1]),delim[0]);
              break;

              case 8:                       // "dcols"
                done=dcols(line,atoi(argv[tab_com[j+1]+1]),atoi(argv[tab_com[j+1]+2]),delim[0]);
              break;
        }
      }
    }


    row[0]='\0';
    if (index > 11)     //only when data command occurs
    {
      switch (rs)       //switch row selection
      {
        case 9:                       // "rows"
          if(rows(row,line,argv[rs_loc+1],argv[rs_loc+2],line_count) == 0)
            return error(0);    //when combination "- M"
        break;

        case 10:                       // "beginswith"
          with_string(row,line,argv[rs_loc+2],atoi(argv[rs_loc+1]),delim[0],-1);
        break;

        case 11:                       // "contains"
          with_string(row,line,argv[rs_loc+2],atoi(argv[rs_loc+1]),delim[0],1);
        break;

        default:                      // when all rows are included
          strcpy(row,line);
      }
      if (row[0] != '\0')         //row selections chooses the rows
      {
        switch (index)
        {
          case 12:                       // "cset"
            done=cset(row,argv[loc+2],atoi(argv[loc+1]),delim[0]);
          break;

          case 13:                       // "tolower"
            done=lower_upper(row,atoi(argv[loc+1]),delim[0],-1);
          break;

          case 14:                       // "toupper"
            done=lower_upper(row,atoi(argv[loc+1]),delim[0],1);
          break;

          case 15:                       // "round"
            done=int_round(row,atoi(argv[loc+1]),delim[0],1);
          break;

          case 16:                       // "int"
            done=int_round(row,atoi(argv[loc+1]),delim[0],-1);
          break;

          case 17:                       // "copy"
            done=copy_swap(row,atoi(argv[loc+1]),atoi(argv[loc+2]),delim[0],-1);
          break;

          case 18:                       // "swap"
            done=copy_swap(row,atoi(argv[loc+1]),atoi(argv[loc+2]),delim[0],1);
          break;

          case 19:                       // "move"
            done=move(row,atoi(argv[loc+1]),atoi(argv[loc+2]),delim[0]);
          break;

          case 20:                       // "csum"
            done=cnum(row,atoi(argv[loc+1]),atoi(argv[loc+2]),atoi(argv[loc+3]),delim[0],1);
          break;

          case 21:                       // "cavg"
            done=cnum(row,atoi(argv[loc+1]),atoi(argv[loc+2]),atoi(argv[loc+3]),delim[0],2);
          break;

          case 22:                       // "cmin"
            done=cnum(row,atoi(argv[loc+1]),atoi(argv[loc+2]),atoi(argv[loc+3]),delim[0],3);
          break;

          case 23:                       // "cmax"
            done=cnum(row,atoi(argv[loc+1]),atoi(argv[loc+2]),atoi(argv[loc+3]),delim[0],4);
          break;
        }
        memset(line,0,LINE_LENGTH);
        strcpy(line,row);
      }//if (row[0] != '\0')
    }//if (index > 11)

   printf("%s",line);

 }//  end of while

 if (arow > 0)      //appends a row after whole input is processed
 {
  for (int a = 0; a < arow; a++)
  {
    done = irow(cols,delim[0]);
  }
 }

 if (line_count == 0)
  return error(5);

 if ((argc > 3 && done == 0) || (rs == 9 && line_count < atoi(argv[rs_loc+2]))) //when mistake occurs and nothing is done
  return error(3);

 return 0;
}  //end of main


//functioons:

int error(int e)
{
  switch (e)
  {
    case 0:
      fprintf(stderr,"\nWrong data, action undefined..\n");
    break;

    case 1:
      fprintf(stderr,"\nNo arguments were found..\n");
    break;

    case 2:
      fprintf(stderr,"\nWrong data where command was expected..\n");
    break;

    case 3:
      fprintf(stderr,"\nThere is missing or wrong data to process your command properly..\n");
    break;

    case 4:
      fprintf(stderr,"\nColumns must be uniform, action undefined..\n");
    break;

    case 5:
      fprintf(stderr,"\nNo input was found..\n");
    break;
  }
return -1;
}


void subst_delim (char *line, char *delim)
{
  int line_length = strlen(line);
  int delim_length = strlen(delim);
  for (int i = 0; i < line_length; i++)
  {
      for (int j = 0; j < delim_length; j++) {
          if(line[i]== delim[j]){
            line[i]= delim[0];
          }
      }
  }
  return;
}

int col_count(char *line, char delim)
{
  int columns = 0;
  int line_length = strlen(line);
  for (int i = 0; i < line_length; i++)
  if (line[i] == delim || line[i] == '\n')
    columns++;
  return columns;
}


int irow(int columns,char delim)
{
  for (int i = 1; i < columns; i++)
    printf("%c", delim);

  printf("\n");
  return 1;
}



int dcol(char *line,int column,char delim)
{
  int line_length = strlen(line);
  char orig_line[line_length];
  strcpy(orig_line,line);
  memset(line,0,line_length);
  int x=0;
  int col_count=0;
  int c=0;
  char cell[CELL_LENGTH];
  for (int l = 0; l < line_length; l++) //within a cell
  {
    if (orig_line[l] != delim && orig_line[l] != '\n')
    {
      cell[c]=orig_line[l];
      c++;
    }
    else                              //end of cell
    {
      col_count++;

      if(col_count != column)
      {
        cell[c]=orig_line[l];
        cell[c+1]='\0';
        strcat (line,cell);
      }
      else                  //deletes a cell
      {
        x=1;
        if (orig_line[l] == '\n')
        {
          line[strlen(line)-1]='\0';
          strcat (line,"\n");
        }
      }
      c=0;
      memset(cell,0,CELL_LENGTH);
    }//else
  }//for
  return x;
}


int dcols(char *line,int from_column,int to_column,char delim)
{
    int line_length = strlen(line);
    char orig_line[line_length];
    strcpy(orig_line,line);
    memset(line,0,line_length);
    int x = 0;
    int col_count = 0;
    int c = 0;
    char cell[CELL_LENGTH];
    for (int l = 0; l < line_length; l++)
    {
      if (orig_line[l] != delim && orig_line[l] != '\n')
      {
        cell[c]=orig_line[l];
        c++;
      }
      else          //end of cell
      {
        col_count++;
        if(col_count < from_column || col_count > to_column)
        {
          cell[c]=orig_line[l];
          cell[c+1]='\0';
          strcat (line,cell);
        }
        else
        {
          x = 1;
          if (orig_line[l] == '\n')
          {
            line[strlen(line)-1]='\0';
            strcat (line,"\n");
          }
        }
        c = 0;
        memset(cell,0,CELL_LENGTH);
      }//else
    }//for
    if (to_column > col_count)
      x = 0;
  return x;
}



int icol(char *line,int column,char delim)
{
    int line_length = strlen(line);
    char orig_line[line_length];
    strcpy(orig_line,line);
    memset(line,0,line_length);
    int x=0;
    int col_count=0;
    int c=0;
    char cell[CELL_LENGTH];
    for (int l = 0; l < line_length; l++)
    {
      if (orig_line[l] != delim && orig_line[l] != '\n')
      {
        cell[c]=orig_line[l];
        c++;
      }
      else        //end of cell
      {
        col_count++;
        if (col_count == column)
        {
         strcat(line,&delim); //line[strlen(line)]=delim
         x=1;
        }
        cell[c]=orig_line[l];
        cell[c+1]='\0';
        strcat (line,cell);
        c=0;
        memset(cell,0,CELL_LENGTH);
      }
    }
  return x;
}



int rows(char *row,char *line,char *N_row,char *M_row,int line_count)
{
  int x=0;
  if ((int)(N_row[0]) != '-' && (int)(M_row[0]) == '-')     //  " N - "
  {
    if (line_count >= atoi(N_row))
      strcpy(row,line);

    x=1;
  }
  else if ((int)(N_row[0]) != '-' && (int)(M_row[0]) != '-')     //  " N M "
  {
    if (line_count >= atoi(N_row) && line_count <= atoi(M_row))
      strcpy(row,line);

    x=1;
  }
  else if ((int)(N_row[0]) == '-' && (int)(M_row[0]) == '-')     //  " - - "
  {
    char c;
    if((c=getc(stdin)) == EOF)
      strcpy(row,line);

    ungetc(c,stdin);
    x=1;
  }
  return x;
}



void with_string(char *row,char *line,char *string,int column,char delim,int down_up)
{
  int line_length = strlen(line);
  int str_length = strlen(string);
  int length;
  char cell[CELL_LENGTH];
  int s=0;
  int c=0;
  int col_count=0;
  for (int l = 0; l < line_length; l++)
  {
    if (line[l] != delim && line[l] != '\n')
    {
      cell[c]=line[l];
      c++;
    }
    else        //end of cell
    {
      col_count++;
      if(col_count == column)
      {
        if (down_up < 0)                    //beginswith
        {
          for (int i = 0; i < str_length; i++)
          {
            if(string[i]==cell[i])
              s++;
          }
        }
        else if (down_up > 0)             //contains
        {
          length=strlen(cell);
          for (int i = 0; i < length; i++)
          {
            if (cell[i] == string[0])
            {
              for (int j = 0; j < str_length; j++)
              {
                if(string[j]==cell[i+j])
                    s++;
              }
              break;    //break when string is found
            }
          }
        }
      }//if
      c=0;
      memset(cell,0,CELL_LENGTH);
    }//else
  }//for

  if (s == str_length)
  {
    strcpy(row,line);
  }
}



int cset(char *line,char *string,int column,char delim)
{
    int line_length = strlen(line);
    char orig_line[line_length];
    strcpy(orig_line,line);
    memset(line,0,line_length);
    int x=0;
    int col_count=0;
    int c=0;
    char cell[CELL_LENGTH];
    for (int l = 0; l < line_length; l++)
    {
      if (orig_line[l] != delim && orig_line[l] != '\n')
      {
        cell[c]=orig_line[l];
        c++;
      }
      else
      {
        col_count++;
        if (col_count == column)
        {
          strcat(line,string);
          line[strlen(line)]=orig_line[l];
          x=1;
        }
        else
        {
          cell[c] = orig_line[l];
          cell[c+1] = '\0';
          strcat (line,cell);
        }
        c=0;
        memset(cell,0,CELL_LENGTH);
      }
    }
  return x;
}


int lower_upper(char *line,int column,char delim,int down_up)
{

  int line_length = strlen(line);
  char orig_line[line_length];
  strcpy(orig_line,line);
  memset(line,0,line_length);
  int x=0;
  int col_count=0;
  int c=0;
  char cell[CELL_LENGTH];
  for (int l = 0; l < line_length; l++)
  {
    if (orig_line[l] != delim && orig_line[l] != '\n')
    {
      cell[c]=orig_line[l];
      c++;
    }
    else
    {
      col_count++;
      if (col_count == column)
      {
          int cell_length=strlen(cell);
        if (down_up<0)                       //"tolower"
        {
          for (int i = 0; i < cell_length; i++)
          {
            if (cell[i]>=65 && cell[i]<=90)
            {
              cell[i]=cell[i]+32;
            }
          }
        }
        else if (down_up>0)                   //"toupper"
        {
          for (int i = 0; i < cell_length; i++)
          {
            if (cell[i]>=97 && cell[i]<=122)
            {
              cell[i]=cell[i]-32;
            }
          }
        }
        x=1;
      }
      cell[c] = orig_line[l];
      cell[c+1] = '\0';
      strcat (line,cell);

      c=0;
      memset(cell,0,CELL_LENGTH);
    }
  }
  return x;
}

int int_round(char *line,int column,char delim,int down_up)
{

  int line_length = strlen(line);
  char orig_line[line_length];
  strcpy(orig_line,line);
  memset(line,0,line_length);
  int x=0;
  int col_count=0;
  int c=0;
  int int_num;
  double round_num;
  char cell[CELL_LENGTH];
  char temp_cell[CELL_LENGTH];
  char* rest;
  for (int l = 0; l < line_length; l++)
  {
    if (orig_line[l] != delim && orig_line[l] != '\n')
    {
      cell[c]=orig_line[l];
      c++;
    }
    else
    {
      col_count++;
      if (col_count != column)
      {
        cell[c] = orig_line[l];
        cell[c+1] = '\0';
        strcat (line,cell);
      }
      else
      {
        if (down_up < 0)                //  "int"
        {
          int_num=(int)strtod(cell,&rest);
          sprintf(temp_cell,"%i",int_num);
        }
        else if (down_up > 0)           //  "round"
        {
          round_num=strtod(cell,&rest);
          sprintf(temp_cell,"%.0f",round_num);
        }
        if (strlen(rest)>0)
        {
          x=0;
          strcat(temp_cell,rest);
        }
        else
          x=1;
        strcat (line,temp_cell);
        c=strlen(line);
        line[c]=orig_line[l];
        line[c+1]='\0';
        memset(temp_cell,0,CELL_LENGTH);
      }
      c=0;
      memset(cell,0,CELL_LENGTH);
    }
  }
  return x;
}



int copy_swap(char *line,int N_column,int M_column,char delim,int down_up)
{
  int line_length = strlen(line);
  char orig_line[line_length];
  strcpy(orig_line,line);
  memset(line,0,line_length);

  int x=0;
  int col_count=0;
  int c=0;
  char cell[CELL_LENGTH];
  char N_cell[CELL_LENGTH]="\0";
  char M_cell[CELL_LENGTH]="\0";

  for (int l = 0; l < line_length; l++)
  {
    if (orig_line[l] != delim && orig_line[l] != '\n')
    {
      cell[c]=orig_line[l];
      c++;
    }
    else        //end of cell
    {
      col_count++;
      if(col_count != N_column && col_count != M_column)
      {
        cell[c]=orig_line[l];
        cell[c+1]='\0';
        strcat (line,cell);
      }
      else if (col_count == N_column)   // N_column
      {
        if (down_up < 0)  // "copy"
        {
          cell[c]=orig_line[l];
          cell[c+1]='\0';
          strcat (line,cell);
        }
        else if (down_up > 0) // "swap"
        {
          M_foo(M_cell,orig_line,delim,M_column);
          c=strlen(M_cell);
          M_cell[c]=orig_line[l];
          M_cell[c+1]='\0';
          strcat (line,M_cell);
        }
      }
      else if (col_count == M_column)   // M_column
      {
        N_foo(N_cell,orig_line,delim,N_column);
        c=strlen(N_cell);
        N_cell[c]=orig_line[l];
        N_cell[c+1]='\0';
        strcat (line,N_cell);
        x=1;
      }
      c=0;
      memset(cell,0,CELL_LENGTH);
    }//else
  }//for
  return x;
}




int move(char *line,int N_column,int M_column,char delim)
{
  int line_length = strlen(line);
  char orig_line[line_length];
  strcpy(orig_line,line);
  memset(line,0,line_length);

  int x=0;
  int col_count=0;
  int c=0;
  char cell[CELL_LENGTH];
  char N_cell[CELL_LENGTH]="\0";

  for (int l = 0; l < line_length; l++)
  {
    if (orig_line[l] != delim && orig_line[l] != '\n')
    {
      cell[c]=orig_line[l];
      c++;
    }
    else        //end of cell
    {
      col_count++;
      cell[c]=orig_line[l];
      cell[c+1]='\0';
      if(col_count != N_column && col_count != M_column)
      {
        strcat (line,cell);
      }
      else if (col_count == N_column)   // N_column
      {                               //delete a cell
        if (orig_line[l] == '\n')
        {
          line[strlen(line)-1]='\0';
          strcat (line,"\n");
        }
      }
      else if (col_count == M_column)   // M_column
      {
        N_foo(N_cell,orig_line,delim,N_column);
        c=strlen(N_cell);
        N_cell[c]=delim;
        N_cell[c+1]='\0';
        strcat (line,N_cell);
        strcat (line,cell);
        x=1;
      }
      c=0;
      memset(cell,0,CELL_LENGTH);
    }//else
  }//for
  return x;
}



void N_foo(char *N_cell,char *line,char delim,int N_column)
{                                                 //only gets N_cell
  int line_length = strlen(line);
  char cell[CELL_LENGTH];
  int c=0;
  int col_count=0;
  for (int l = 0; l < line_length; l++)
  {
    if (line[l] != delim && line[l] != '\n')
    {
      cell[c]=line[l];
      c++;
    }
    else        //end of cell
    {
      col_count++;

      if(col_count == N_column)
        strcpy(N_cell,cell);

      c=0;
      memset(cell,0,CELL_LENGTH);
    }
  }
}



void M_foo(char *M_cell,char *line,char delim,int M_column)
{                                                 //only gets M_cell
  int line_length = strlen(line);
  char cell[CELL_LENGTH];
  int c=0;
  int col_count=0;
  for (int l = 0; l < line_length; l++)
  {
    if (line[l] != delim && line[l] != '\n')
    {
      cell[c]=line[l];
      c++;
    }
    else        //end of cell
    {
      col_count++;

      if(col_count == M_column)
        strcpy(M_cell,cell);

      c=0;
      memset(cell,0,CELL_LENGTH);
    }
  }
}



 //csum,cavg,cmin and cmax proceeds the first part of cell composed of float, when begins with string it is taken as zero
int cnum(char *line,int column,int N_column,int M_column,char delim,int click)
{
  int line_length = strlen(line);
  int length = M_column-N_column+1;
  char orig_line[line_length];
  strcpy(orig_line,line);
  memset(line,0,line_length);
  int x=0;
  int col_count=0;
  int c=0;
  double result=0;
  char string[CELL_LENGTH];
  char cell[CELL_LENGTH];
  double array[LINE_LENGTH];
  for (int l = 0; l < line_length; l++)
  {
    if (orig_line[l] != delim && orig_line[l] != '\n')
    {
      cell[c]=orig_line[l];
      c++;
    }
    else         // end of cell
    {
      col_count++;

      if (col_count == column)
      {
        numbers(array,orig_line,N_column,M_column,delim);

        if (click > 2)
          result=array[0];

        for (int i = 0; i < length; i++)
        {
          switch (click)
          {
            case 1:             //csum
            case 2:             //cavg
                result = result + array[i];
            break;

            case 3:             //cmin
                if (array[i] < result)
                  result = array[i];
            break;

            case 4:             //cmax
                if (array[i] > result)
                  result = array[i];
            break;
          }//switch
        }//for

        if (click == 2)
          result=result/length;

        sprintf(string,"%g",result);
        strcat(line,string);
        line[strlen(line)]=orig_line[l];
        x=1;
      }//if
      else
      {
        cell[c] = orig_line[l];
        cell[c+1] = '\0';
        strcat (line,cell);
      }
      c=0;
      memset(cell,0,CELL_LENGTH);
    }//else
  }//for
return x;
}



void numbers(double *array,char *line,int N_column,int M_column,char delim)
{                                                 //only gets numbers needed
  int line_length = strlen(line);
  int col_count=0;
  int c=0;
  double number;
  char cell[CELL_LENGTH];
  int a=0;
  for (int l = 0; l < line_length; l++)
  {
    if (line[l] != delim && line[l] != '\n')
    {
      cell[c]=line[l];
      c++;
    }
    else
    {
      col_count++;

      if (col_count >= N_column && col_count <= M_column )
      {
        number=strtod(cell,NULL);
        array[a]=number;
        a++;
      }
      c=0;
      memset(cell,0,CELL_LENGTH);
    }
  }
}

//      **sorry for lack of comments, but I'm already real tired**

//                **THE END**   (thanks.. for patience :3)

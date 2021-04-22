/*

** project 2: Data Structures

** author: Natalia Bubakova
** login: xbubak01

** group: tpl 30_Po 15:00 - 16:50, lab. N103, as. Husa

** date: 6 December 2020
** academic year: 2020/2021

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 1000

    /*** structures ***/

typedef struct cell
{
  char *string;
  int str_n;
}
  CELL;


typedef struct row
{
  CELL *cells;
  int cell_n;
}
  ROW;


typedef struct table
{
  ROW *rows;
  int rows_num;
  int cols_num;
}
  TABLE;


    /*** declaration of functions ***/

int error(int e);
void load_tab(TABLE *table, FILE *file, char *delim);
void load_str(TABLE *table, FILE *file, char *delim);
void check_tab(TABLE *table);
int process(TABLE *table, char *cmd_seq);
int sel_cells(TABLE *table, TABLE *select, TABLE *sel_end, char *cmd_seq, int i);
int do_cmd(TABLE *table, TABLE *select, TABLE *sel_end, char *cmd_str, char *cmd_seq, int i);
void def(TABLE *table, TABLE *select, TABLE *sel_end, char *data);



            /*** main ***/

int main(int argc, char *argv[])
{
  TABLE table = {NULL, 0, 0};
  int e;
  char *delim;


  if (argc < 3)
    return error(1);
  else if (strcmp(argv[1],"-d") == 0 && argc == 5)
    delim = argv[2];
  else if (strcmp(argv[1],"-d") != 0 && argc == 3)
      delim = " ";
  else
    return error(0);



  FILE *file = fopen(argv[argc-1], "r");
  if (file != NULL)
    load_tab(&table,file,delim);
  else
    return error(2);
  fclose(file);

  char *cmd_seq = argv[argc-2];
  e = process(&table,cmd_seq);
  if (e < 0)
    return error(0);

          /*** prints table ***/

  for(int i = 0; i < table.rows_num; i++){
    for(int j = 0; j < table.cols_num; j++)
    {
      if(table.rows[i].cells[j].string != NULL)
      {
        printf("%s", table.rows[i].cells[j].string);
        free(table.rows[i].cells[j].string);
      }
      if(j < table.rows[i].cell_n-1)    //no delim at the end of row
        printf("%c",delim[0]);          //delim at the end of cell
    }
    printf("\n");                      //new row
    if(table.rows[i].cells != NULL)
      free(table.rows[i].cells);
  }
  free(table.rows);

  return 0;
}


          /*** functions ***/



int error(int e)
{
  switch (e)
  {
    case 0:
      fprintf(stderr,"\nWrong argument\n");
    break;

    case 1:
      fprintf(stderr,"\nMissing arguments..\n");
    break;

    case 2:
      fprintf(stderr,"\nOpening the file failed..\n");
    break;

  }
return -1;
}



            /***prepares allocation for cells***/
void load_tab(TABLE *table, FILE *file, char *delim)
{
  char c;
  while((c = fgetc(file)) != EOF)
  {
      if(c == '\n' || table->rows_num == 0)       //new row expected
      {
        if(table->rows_num == 0)
        {
            ungetc(c,file);                       //so it can be processed in another function
        }
        else if(table->rows[table->rows_num - 1].cell_n > table->cols_num)
        {                                         //max n of cells in a row = n of cols
          table->cols_num = table->rows[table->rows_num - 1].cell_n;
        }

        table->rows_num++;                                   //creates new row
        table->rows = realloc(table->rows, table->rows_num * sizeof(ROW));
        table->rows[table->rows_num - 1].cell_n = 1;         //sets first cell in a row
        table->rows[table->rows_num - 1].cells = malloc(sizeof(CELL));
        load_str(table,file,delim);
      }
      else if( strchr(delim, c) != NULL )
      {
        table->rows[table->rows_num - 1].cell_n++;
        table->rows[table->rows_num - 1].cells = realloc(table->rows[table->rows_num - 1].cells,table->rows[table->rows_num - 1].cell_n * sizeof(CELL));
      }

      else
      {
        ungetc(c,file);
        load_str(table,file,delim);
      }
  }
}


            /***loads string into prepared cells***/
void load_str(TABLE *table, FILE *file, char *delim)
{
  CELL cell = {'\0', 1};
  char c;

  while((c = fgetc(file)) != EOF)
  {
    cell.string = realloc(cell.string, cell.str_n * sizeof(char));

    if( strchr(delim, c) != NULL  ||  c == '\n')
    {
      cell.string[cell.str_n-1]= '\0';
      table->rows[table->rows_num - 1].cells[table->rows[table->rows_num - 1].cell_n - 1] = cell;
      ungetc(c,file);
      return;
    }
    else
    {
      cell.string[cell.str_n - 1] = c;
      cell.str_n++;
    }
  }

  if((c = getc(file)) == EOF)
  {
    free(table->rows[table->rows_num - 1].cells);
    table->rows_num--;
    check_tab(table);
  }
}

          /***fixes empty cells***/
void check_tab(TABLE *table)
{
  for (int i = 0; i < table->rows_num; i++)
  {
    if (table->rows[i].cell_n < table-> cols_num)
      table->rows[i].cells = realloc(table->rows[i].cells, (table->rows[i].cell_n + (table-> cols_num - table->rows[i].cell_n)) * sizeof(CELL));
  }
}


      /***processes selection and commands***/
int process(TABLE *table, char *cmd_seq)
{
  TABLE select = {NULL,0,0};
  TABLE sel_end = {NULL,0,0};
  char cmd_str[N];
  int n = 0;
  char c;
  int length = strlen(cmd_seq);
  int i = 0;
  strcat(cmd_seq,";");
  while (i <= length)
  {
    c = cmd_seq[i];
    if (c == '[')
    {
      select.rows_num = 0;
      select.cols_num = 0;
      sel_end.rows_num = 0;
      sel_end.cols_num = 0;
      i = sel_cells(table,&select,&sel_end,cmd_seq,i);
      //printf("selection: [%i, %i; %i, %i]\n", select->rows_num, select->cols_num, sel_end->rows_num, sel_end-> cols_num);
      if (i < 0)
        return -1;
    }
    else
    {
      if (c != ';' && c != ' ')
      {
        cmd_str[n] = c;
        n++;
      }
      else if (cmd_str[0] != '\0' && (c == ';' || c == ' '))
      {
        cmd_str[n] = '\0';
        i = do_cmd(table,&select,&sel_end,cmd_str,cmd_seq,i);
        if (i < 0)
          return -1;
        memset(cmd_str,0,n+1);
        n = 0;
      }
    }
    i++;
  }
  return 0;
}


                        /***selection***/

int sel_cells(TABLE *table, TABLE *select, TABLE *sel_end, char *cmd_seq, int i)
{
  char *cmnds[5]={"_", "min", "max", "find", "set"};
  char str[N];
  int n = 0;
  char num_str[N];
  int m = 0;
  i++;
  while (cmd_seq[i] != ']')
  {
    str[n] = cmd_seq[i];
    n++;
    i++;
  }
  str[n] = '\0';
  if (strchr(str, ',') == 0)
  {
    for (int j = 0; j < 5; j++)
    {
      if (strcmp(str,cmnds[j]))
      {
        switch (j)
        {
          // case 0:                       // "_"
          //   printf("%s\n", cmnds[j]);//
          // break;
          //
          // case 1:                       // "min"
          //   printf("%s\n", cmnds[j]);//
          // break;
          //
          // case 2:                       // "max"
          //   printf("%s\n", cmnds[j]);//
          // break;
          //
          // case 3:                       // "find"
          //   printf("%s\n", cmnds[j]);//
          // break;
          //
          // case 4:                       // "set"
          //   printf("%s\n", cmnds[j]);//
          // break;
        }
        return i+1;
      }
    }
  }
  else
  {
    for (int j = 0; j <= n; j++)
    {
      if (str[j] != ',' && str[j] != '\0')
      {
        num_str[m] = str[j];
        m++;
      }
      else
      {
        num_str[m] = '\0';
        m = 0;
        if (select->rows_num == 0)                            //[R,c]
        {
          if (strcmp(num_str,"_") == 0)
          {
            select->rows_num = 1;
            sel_end->rows_num = table->rows_num;
          }
          else
          {
            select->rows_num = atoi(num_str);
            sel_end->rows_num = atoi(num_str);
          }
          memset(num_str,0,m+1);
          m = 0;
        }
        else if (select->cols_num == 0)                      //[r,C]
        {
          if (strcmp(num_str,"_") == 0)
          {
            select->cols_num = 1;
            sel_end->cols_num = table->cols_num;
          }
          else
          {
            select->cols_num = atoi(num_str);
            sel_end->cols_num = atoi(num_str);
          }
          memset(num_str,0,m+1);
          m = 0;
        }
        else if (sel_end->rows_num == 0 || sel_end->rows_num == select->rows_num)
        {                                                    //[r,c,R,c]
          sel_end->rows_num = atoi(num_str);
          memset(num_str,0,m+1);
          m = 0;
        }
        else if (sel_end->cols_num == 0 || sel_end->cols_num == select->cols_num)
        {                                                   // [r,c,r,C]
          sel_end->cols_num = atoi(num_str);
          memset(num_str,0,m+1);
          m = 0;
        }
      }

    }
    return i+1;
  }
  return -1;
}







int do_cmd(TABLE *table, TABLE *select, TABLE *sel_end, char *cmd_str, char *cmd_seq, int i)
{
  char *commands[16]={"irow", "arow", "drow", "icol", "acol", "dcol",  "set", "clear", "swap", "sum", "avg", "count", "len",  "def", "use", "inc"};
  char data[N];
  int n = 0;
  if (cmd_seq[i] == ' ')
  {
    i++;
    while (cmd_seq[i] != ';')
    {
      data[n] = cmd_seq[i];
      i++;
      n++;
    }
    data[n] = '\0';
  }

  for (int j = 0; j < 16; j++)
  {
    if (strcmp(cmd_str,commands[j]) == 0)
    {
      switch (j)
      {
        // case 0:                 // "irow"
        //   printf("%s\n", commands[j]);
        // break;
        //
        // case 1:                 // "arow"
        //   printf("%s\n", commands[j]);
        // break;
        //
        // case 2:                 // "drow"
        //   printf("%s\n", commands[j]);
        // break;
        //
        // case 3:                 // "icol"
        //   printf("%s\n", commands[j]);
        // break;
        //
        // case 4:                 // "acol"
        //   printf("%s\n", commands[j]);
        // break;
        //
        // case 5:                 // "dcol"
        //   printf("%s\n", commands[j]);
        // break;
        //
        // case 6:                 //"set"
        //   printf("%s\n", commands[j]);
        // break;
        //
        // case 7:                // "clear"
        //   printf("%s\n", commands[j]);
        // break;
        //
        // case 8:                // "swap"
        //   printf("%s\n", commands[j]);
        // break;
        //
        // case 9:                // "sum"
        //   printf("%s\n", commands[j]);
        // break;
        //
        // case 10:               // "avg"
        //   printf("%s\n", commands[j]);
        // break;
        //
        // case 11:              // "count"
        //   printf("%s\n", commands[j]);
        // break;
        //
        // case 12:              // "len"
        //   printf("%s\n", commands[j]);
        // break;

        case 13:              // "def"
          def(table,select,sel_end,data);
        break;
        //
        // case 14:              // "use"
        //   printf("%s\n", commands[j]);
        // break;
        //
        // case 15:              // "inc"
        //   printf("%s\n", commands[j]);
        // break;
      }
      return i;
    }
  }
return -1;
}


void def(TABLE *table, TABLE *select, TABLE *sel_end, char *data)
{
  int n = strlen(data);
  char num_str[N];
  for (int i = 1; i < n; i++)
  {
    num_str[i-1] = data[i];
    if (i == n-1)
      num_str[i] = '\0';
  }
  n = atoi(num_str);            //number of variable
  if (n >=0 && n < 10)
  {
    select->rows = malloc(sizeof(ROW));
    select->rows[0].cells = malloc(10 * sizeof(CELL));

    if (select->rows_num <= table->rows_num && select->cols_num <= table->cols_num)
    {
      select->rows[0].cells[n-1].string = table->rows[select->rows_num-1].cells[select->cols_num - 1].string;
      //printf("defined cell: %s\n", select->rows[0].cells[n-1].string);
    }
  }
}


          /*** work in progress ***/

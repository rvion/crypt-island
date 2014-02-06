

#include <stdio.h>      /* Stdio        */
#include <stdlib.h>     /* Stdlib       */
#include <string.h>     /* for strlen   */

#define SIZE_MAX 50
#define LINE_LENGTH 1024
#define DEBUG 0
#define TRUE 1
#define FALSE 0

int nb_solutions;

/*--- PROTOTYPES -------------------------------------------------------*/
void write_board_in_output_file(FILE* output_file, int size, int **solution_gen);
int* get_values_ints(char* original, int* nb_values_s, int* nb_remaining_sp, int size);
int test_sol();
void try( int **solution_ref, int **solution_gen, int *ind_sol
        , int rem_islands, int rem_spaces, int rem_column, int rem_row
		, int size
		, int  *base_num, int  *base_space, int **base_input
		, FILE *output_file );

/*--- IMPLEMENTATIONS --------------------------------------------------*/
void write_board_in_output_file(FILE* output_file, int size, int **solution_gen)
{
    int i,j,p,q,temp;
    int **board;
    nb_solutions++;
    board = (int**) malloc(size * sizeof(int*));
    for (j=0; j<size; j++)
        board[j] = (int*) malloc(size * sizeof(int));

    for (j=0; j<size; j++)
    {
        p=0;
        q=0;

        while (q !=size)
        {
            temp = solution_gen[j][p];
            if (temp<0) {temp = -temp;}
            for (i=0; i<temp; i++)
            {
                board[j][q]=solution_gen[j][p];
                q++;
            }
            p++;
        }
    }
    if (DEBUG == TRUE)
    {
        printf("\n");
        for (i=0; i<size; i++)
        {
            for (j=0; j<size; j++)
            {
                if (board[j][i]<0)
                     { printf("- "); }
                else { printf("* "); }
            }
            printf("\n");
        }
        printf("$$$$$$$$$$\n");
    }
    for (i=0; i<size; i++)
    {
        for (j=0; j<size; j++)
        {
            if (board[j][i]<0)
                 { fprintf(output_file,"-"); }
            else { fprintf(output_file, "*"); }
            if (j!=size-1) {fprintf(output_file," ");}
        }
        fprintf(output_file,"\n");
    }
    fprintf(output_file,"$$$$$$$$$$\n");

	/* Print element in OUTPUT file */
	/*fprintf(output_file, "%d\n",size);
    fprintf(output_file, "$$$$$$$$$$\n");*/
}

int* get_values_ints(char* original, int* nb_values_s, int* nb_remaining_sp, int size)
{
	int *values = NULL;
	int sum = 0;
	int nb_values =0;
	int i = 0;
	int j = 0;
	int temp = 0;

    for(i = 0; i < strlen(original); i++)
	{
        /*if(original[i] == ' ' || original[i] == '\n'|| i == strlen(original)-1)*/
        if(original[i] >='0' && original[i] <='9')
		{
			if (temp == 0)
				{nb_values++; temp = 1;}
		}
		else {temp = 0;}
	}

	values = calloc(nb_values, sizeof(int));
	*nb_values_s = nb_values-1;
	if (DEBUG == TRUE) printf("nb_values= %d; [",nb_values-1);

	i=0;

	while((i < LINE_LENGTH) && 	(j < nb_values) && 	(original[i]!= '\n'))
	{
		values[j] = atoi((const char *) &original[i]);
		if (DEBUG == TRUE) printf("%d- ",values[j]);

		sum += values[j];
		j++;
		if (j == nb_values)
		{
            sum += (nb_values-2);
            if (nb_values-1 == 0) {*nb_remaining_sp = size;}
            else                  {*nb_remaining_sp = size - sum;}

            if (DEBUG == TRUE) printf("]; rem_space= %d;\n",*nb_remaining_sp);/*size - sum);*/
		    return values;
        }
        i++;
		while (  (original[i] != ' ') && (original[i] != '\n')) i++;
	}
	if (DEBUG == TRUE) printf("\n\nERROR !!\n\n");
	return values;
}

int main()
{
	/* DECLARTION ------------------------------------------------------*/
    int num, ind;
	int i,j;
    char* buffer;               /* Buffer to read Ints                  */
	int size;                   /* size of the board                    */
	int c;

	FILE  *tmp = tmpfile();
	FILE  *input_file;   /* INPUT  file  								*/
	FILE  *output_file;  /* OUTPUT file  								*/

	int **d_hor_input;	/* horizontal lines from input file 			*/
	int  *d_hor_num;  	/* number of island on each hor line		    */
	int  *d_hor_space;  /* number of remaining spaces on each hot line	*/
	int **d_ver_input;	/* vertical lines from input file 				*/
	int  *d_ver_num;  	/* number of island on each ver line    		*/
	int  *d_ver_space;  /* number of remaining spaces on each ver line	*/

    int  *len_sol_ref;
	int **solution_ref; /* what should be the solution generated        */
	int **solution_gen;/* The encoded solution being tested        	*/
	int  *ind_sol;     	/* indices of next place to write   			*/

    int **base_input;
    int  *base_num;
    int  *base_space;

    int **sol_input;
    int  *sol_num;
    int  *sol_space;

	/* INITIALISATION --------------------------------------------------*/
    num  = 0;
    size = 0;
	ind  = 0;
	nb_solutions = 0;

    buffer       = (char*)  malloc(LINE_LENGTH * sizeof(int*));

    d_hor_input  = (int**)  malloc(SIZE_MAX * sizeof(int*));
    d_hor_num    = (int*)   calloc(SIZE_MAX , sizeof(int));
    d_hor_space  = (int*)   calloc(SIZE_MAX , sizeof(int));

    d_ver_input  = (int**)  malloc(SIZE_MAX * sizeof(int*));
    d_ver_num    = (int*)   calloc(SIZE_MAX , sizeof(int));
    d_ver_space  = (int*)   calloc(SIZE_MAX , sizeof(int));

    len_sol_ref  = (int*)   calloc(SIZE_MAX , sizeof(int));
    solution_ref = (int**)  malloc(SIZE_MAX * sizeof(int*));
    solution_gen = (int**)  malloc(SIZE_MAX * sizeof(int*));
    ind_sol      = (int*)   calloc(SIZE_MAX , sizeof(int));

	input_file   = fopen("in.txt",  "r");
    output_file  = fopen("out.txt", "w+");

    for (ind =0; ind < SIZE_MAX; ind++)
    {
        solution_ref[ind] = (int*) calloc(SIZE_MAX, sizeof(int));
        solution_gen[ind] = (int*) calloc(SIZE_MAX, sizeof(int));
    }

	/* GETTING DATAS ---------------------------------------------------*/
	if (DEBUG == TRUE) printf("\nREADING COMPACT ISLAND DESCRIPTION\n\n");
	fscanf(input_file, "%d\n", &size);
	if (DEBUG == TRUE) printf("Size : %d\n\n",size);
	fscanf(input_file, "\n");

    for (i=0; i<size; i++)
	{
	    if (DEBUG == TRUE) printf("horizontal data| line %d: ",i);
	    fgets(buffer, LINE_LENGTH, input_file);
	    d_hor_input[i] = get_values_ints(buffer, &d_hor_num[i], &d_hor_space[i], size);
	}
	fscanf(input_file, "\n");
	if (DEBUG == TRUE) printf("\n\n");
    for (i=0; i<size; i++)
	{
	    if (DEBUG == TRUE) printf("vertical data  | line %d: ",i);
	    fgets(buffer, LINE_LENGTH, input_file);
	    d_ver_input[i] = get_values_ints(buffer, &d_ver_num[i], &d_ver_space[i], size);
	}

    base_input = d_hor_input;
    base_num   = d_hor_num;
    base_space = d_hor_space;

    sol_input = d_ver_input;
    sol_num   = d_ver_num;
    sol_space = d_ver_space;

    for (i=0; i< size; i++)
    {
        solution_ref[i][0] = 0;
        solution_ref[i][1] = sol_input[i][0];
    }
    for (i=0; i<size; i++)
    for (j=1; j<sol_num[i]; j++)
    {
        solution_ref[i][2*j] = -1;
        solution_ref[i][2*j+1]   = sol_input[i][j];
    }
    if (DEBUG == TRUE)
    {
        printf("\nREF:");
        for (i=0; i<size+2; i++)
        {
            printf("\n");
            for (j=0; j<size; j++)
                printf("%d\t",solution_ref[j][i]);
        }
    }
	/* HOW SHOULD SOLUTION_REF LOOKS LIKE
	|    * - * * - -  1 2		-> [0, 1, -1, 2, 0]
	|    - * * * - *  3 1		-> [0, 3, -1, 1, 0]
	|    * - * - * -  1 1 1		-> [0, 1, -1, 1, -1, 1, 0]
	|    - * * * * *  5			-> [0, 5, 0]
	|    * * - * - *  2 1 1		-> [0, 2, -1, 1, -1, 1, 0]
	|    - - - * - -  1			-> [0, 1, 0]
	*/

    /* WE FIND SOLUTIONS */
    if (DEBUG == TRUE) printf("\n\nUNCOMPRESSING ISLANDS..\n\n");
    if (DEBUG == TRUE) {getchar();getchar();}
	try ( solution_ref
        , solution_gen
        , ind_sol
        , base_num[0]
        , base_space[0] /* remaining spaces on this line    */
		, size          /* rem column                       */
		, size          /* rem row                          */
		, size          /* size                             */
		, base_num
		, base_space
		, base_input
		, tmp);

    /* WE WRITE THE NUMBER OF SOLUTIONS FOUNDED IN THE BEGINING OF THE FILE */

	rewind(tmp);
	fprintf(output_file,"%d\n$$$$$$$$$$\n",nb_solutions);
	while((c=fgetc(tmp))!=EOF) {fputc(c, output_file);}
	/*while((c=fgetc(tmp))!=EOF) {fputc(c, output_file);printf("%c",c);}*/

    fclose(tmp);
	/* Reading data from file ------------------------------------------*/
    if (DEBUG == TRUE) printf("\n\nCLOSING FILES");
    fclose(input_file);
    fclose(output_file);
    if (DEBUG == TRUE) printf("\n\nEXIT\n\n");
    return EXIT_SUCCESS;
}

void try( int **solution_ref
        , int **solution_gen
        , int  *ind_sol
        , int rem_islands
		, int rem_spaces
		, int rem_column
		, int rem_row
		, int size
		, int  *base_num
		, int  *base_space
		, int **base_input
		, FILE *output_file)
{
    int undo[50] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int ind_sol_undo[50] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    int solution_gen_undo[50] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


	int i,j,xx, yy, temp,db,db2;
    xx = -999;
    yy = -999;

    for(db=0;db<50;db++)
        ind_sol_undo[db] = ind_sol[db];
    for(db=0;db<50;db++)
        solution_gen_undo[db] = solution_gen[db][ind_sol_undo[db]];

    if (DEBUG == TRUE)
    {
        printf("\n");
        printf("(x=%d,y=%d) ",size - rem_column,size - rem_row);
        for (i=0; i<size - rem_row; i++) {printf("  ");}
        printf("{ ");
    }

	if (rem_row == 0)
	{
	    /* Last check to avoid error to lazy solution testing */
		for (i=0; i<size+2; i++)
		for (j=0; j<size+2; j++)
		{
			if ((   solution_ref[i][j] >0
				 && solution_gen[i][j] >= 0
				 && solution_gen[i][j]< solution_ref[i][j])
			||  solution_gen[i][j]*solution_ref[i][j]<0)
			{
				goto sortie;
			}
		}
		if (DEBUG == TRUE)
        {
            /* ---------- */
            printf(" solution found.. :\n");
            printf("\nGEN:");
            for (i=0; i<size+2; i++)
            {
                printf("\n");
                for (j=0; j<size; j++)
                    printf("%d\t",solution_gen[j][i]);
            }
        }
		write_board_in_output_file(output_file, size, solution_gen);
        if (DEBUG == TRUE)
        {
            printf("\n");
            printf("(x=%d,y=%d) ",size - rem_column,size - rem_row);
            for (i=0; i<size - rem_row; i++) {printf("  ");}
            printf("} ");
        }
		return;
	}
	else if (rem_column == 0)
	{
	    if (DEBUG == TRUE) printf("EOL");


        yy = size - rem_row +1;

        try ( solution_ref, solution_gen
            , ind_sol
            , base_num[yy]      /* remaining islands on this line   */
            , base_space[yy]    /* remaining spaces on this line    */
            , size              /* rem column                       */
            , rem_row -1        /* rem row                          */
            , size              /* size                             */
            , base_num, base_space, base_input
            , output_file);

        if (DEBUG == TRUE)
        {
            printf("\n");
            printf("(x=%d,y=%d) ",size - rem_column,size - rem_row);
            for (i=0; i<size - rem_row; i++) {printf("  ");}
            printf("} ");
        }
		return;
	}
	else
	{
        xx = size - rem_column;
        yy = size - rem_row;
        for (i=0; i<= rem_spaces; i++)
        {
            if (rem_islands==0)  {i=rem_spaces;}
            for (j=0; j<i; j++)
            {
                /* WE PUT THE SPACES */
                temp = xx + j;
                if (DEBUG==TRUE)  printf(".");
                if (solution_gen[temp][ind_sol[temp]] >  0)
                {
                    if (solution_gen[temp][ind_sol[temp]] != solution_ref[temp][ind_sol[temp]])
                    {
                       if (DEBUG == TRUE){printf(" > wrong");}
                        goto sortie;
                    }
                    ind_sol[temp]++;
                    solution_gen[temp][ind_sol[temp]] = -1;
                    undo[temp]=-1; /*-------------------------------------------------------------------------------------*/

                }
                else { solution_gen[temp][ind_sol[temp]]--; undo[temp]=2;/*-------------------------------------------------------------------------------------*/}
            }

            for (j=0; j<base_input[yy][base_num[yy]-rem_islands]; j++)
            {
                /* WE PUT THE NEXT ISLAND*/
                temp = xx+i+j;
                if (DEBUG==TRUE)  printf("*");
                if (solution_gen[temp][ind_sol[temp]] >  0)
                {
                    solution_gen[temp][ind_sol[temp]]++;
                    undo[temp]=-2;/*-------------------------------------------------------------------------------------*/

                    if(solution_gen[temp][ind_sol[temp]] > solution_ref[temp][ind_sol[temp]])
                    {
                        if (DEBUG == TRUE){printf(" > wrong");}
                        goto sortie;
                    }
                }
                else
                {
                    ind_sol[temp]++;
                    solution_gen[temp][ind_sol[temp]] = 1;
                    undo[temp] = -3;/*-3------------------------------------------------------------------------------------*/
                }
            }
            if (rem_islands>=2)
            {
                /* WE PUT THE EXTRA SPACE AFTER ISLAND IF MORE ISLAND TO COME*/
                temp = xx+i+base_input[yy][base_num[yy]-rem_islands];
                if (DEBUG == TRUE) {printf("-");}
                if (solution_gen[temp][ind_sol[temp]] >  0)
                {
                    if (solution_gen[temp][ind_sol[temp]] != solution_ref[temp][ind_sol[temp]])
                    {
                        if (DEBUG == TRUE){printf(" > wrong");}
                        goto sortie;
                    }
                    ind_sol[temp]++;
                    undo[temp]=-1;/*-------------------------------------------------------------------------------------*/
                    solution_gen[temp][ind_sol[temp]] = -1;
                }
                else
                    {solution_gen[temp][ind_sol[temp]]--; undo[temp]=2;/*-------------------------------------------------------------------------------------*/ }
            }
            if (rem_islands>=2) {temp = rem_column-i-base_input[yy][base_num[yy]-rem_islands]-1;}
            else                {temp = rem_column-i-base_input[yy][base_num[yy]-rem_islands];}





	    /*------------------------------------------------------*
	    if (size - rem_row == 0)
	    {
	        printf("\n");
            for (db=0; db<size; db++)
                printf("%d ",ind_sol[db]);
	        printf("\n");
            for (db=0; db<size; db++)
            {

                printf("\n");
                for (db2=0; db2<size; db2++)
                    printf("%d\t",solution_gen[db2][db]);
            }
            printf("\n%d\n\n",rem_spaces-i);
	    }
	    /*------------------------------------------------------*/








            try ( solution_ref, solution_gen, ind_sol, rem_islands-1
                , rem_spaces-i      /* remaining spaces on this line    */
                , temp              /* rem column                       */
                , rem_row           /* rem row                          */
                , size              /* size                             */
                , base_num, base_space, base_input, output_file);

            sortie:


            /*for (j=0;j<size;j++)
            {
                if      (undo[j]==-1){solution_gen[j][ind_sol[j]] = 0; ind_sol[j]--; if (DEBUG == TRUE) printf("^");}
                else if (undo[j]==-2){solution_gen[j][ind_sol[j]] -- ; if (DEBUG == TRUE) printf("^");}
                else if (undo[j]== 2){solution_gen[j][ind_sol[j]] ++ ; if (DEBUG == TRUE) printf("^");}
                else if (undo[j]==-3){solution_gen[j][ind_sol[j]] = 0; ind_sol[j]--; if (DEBUG == TRUE) printf("^");}
            }*/

            for(db=0;db<50;db++)
            {
                ind_sol[db] = ind_sol_undo[db];
                solution_gen[db][ind_sol_undo[db]] = solution_gen_undo[db];
                solution_gen[db][ind_sol_undo[db]+1] = 0;
            }

/*
            for (db=0; db<size; db++)
                if (ind_sol[db] <0)
                {
                    printf("%d----------------------\n",yy);
                    for (db=0; db<size; db++)
                        printf("%d ",undo[db]);
                        printf("\n");
                    for (db=0; db<size; db++)
                        printf("%d ",ind_sol[db]);
                    printf("\n----------------------\n",yy);
                    return;
                }*/
                /*
            for (db=0; db<size; db++)
                if (ind_sol[db] >yy)
                {
                    printf("%d----------oo------------\n",yy);
                    for (db=0; db<size; db++)
                        printf("%d ",undo[db]);
                        printf("\n");
                    for (db=0; db<size; db++)
                        printf("%d ",ind_sol[db]);
                    printf("\n----------oo------------\n",yy);
                    return;
                }*/

        }
	}
    if (DEBUG == TRUE)
    {
        printf("\n");
        printf("(x=%d,y=%d) ",size - rem_column,size - rem_row);
        for (i=0; i<size - rem_row; i++) {printf("  ");}
        printf("} ");
    }
}

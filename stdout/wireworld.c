/* Wireworld - Brian Silverman's cellular automata. */ 

#include <stdio.h>
#include <stdlib.h>

#define GENERATIONS 1000
/* 
 * YS is defined as the number of rows in the grid. 
 * XS is defined as the number columns in the grid. 
 */
#define YS 40
#define XS 40
#define FIRST_COLUMN 0
#define LAST_COLUMN (XS - 1)
#define EMPTY ' '
#define HEAD 'H'
#define TAIL 't'
#define COPPER 'c'

/* Moore neighbourhood: 1 central and 8 surrounding cells.*/
#define MOORE_N 8

/* Used in pointer arithmetic to reach surrounding cells. */
#define NW -(XS + 1)
#define N -XS
#define NE -(XS - 1)
#define W -1
#define E +1
#define SW +(XS - 1)
#define S XS
#define SE +(XS + 1)

/* Bounds search area when copper in first/last column. */
#define SKIP_ALL_WEST_CELLS north
#define SKIP_ALL_EAST_CELLS north_east

/* Evaluates number of heads, n, in surrounding cells. */
#define COPPER_TO_HEAD(n) ((n == 1) || (n == 2))

/* Test for any unexpected/incorrect characters in array. */
#define UNEXPECTED_CHAR(c) ((c != EMPTY) && (c != HEAD) && \
(c != TAIL) && (c != COPPER))

/* Used to bound search area when in top/bottom row. */ 
#define LOWER_CHECK(i, j, m) ((i == 0) && (j + m < 0))
#define UPPER_CHECK(i, j, m) ((i == (YS - 1)) \
&& ((j + m) > YS*XS))

/* True iff attempting to search out-of-bounds of array. */
#define OUT_OF_BOUNDS(i, j, m) (LOWER_CHECK(i, j, m) \
|| UPPER_CHECK(i, j, m))

enum moore_neighbors {north_west, west, south_west, north, \
south, north_east, east, south_east} ; 
typedef enum moore_neighbors moore_neighbors ;

struct alternate_2d_arrays{
  char a[YS][XS] ; 
  char b[YS][XS] ; 
} ; 
typedef struct alternate_2d_arrays alternate_2d_arrays ; 

void open_file(FILE **wwf, char *argv) ;
void check_file_dimensions(FILE *wwf, char *argv) ;
void fill_and_check_array(FILE *wwf, char a[YS][XS]) ;
void check_characters_correct(int u) ; 
void wireworld_iterations(char a[YS][XS], char b[YS][XS]) ;
void print_to_screen(char a[YS][XS]) ;
void rules(char a[YS][XS], char b[YS][XS]) ;
void copper_bounds_check(char *p, char *q, int i, int j) ; 
void copper_next(char *p, char *q, int i, int j, int start,\
int finish) ; 
void close_file(FILE **wwf) ;

int main(int argc, char **argv)
{
  FILE *wwf = NULL ;
  alternate_2d_arrays state ; 
    
  if(argc == 2){
    open_file(&wwf, argv[1]) ;

    check_file_dimensions(wwf, argv[1]) ;
     
    fill_and_check_array(wwf, state.a) ;
        
    wireworld_iterations(state.a, state.b) ;   
  }
  
  else{
    fprintf(stderr, "\nERROR: Unexpected input.\nPlease " \
    "try something like: %s wirefile.txt\n\n", argv[0]) ;
    return 1 ; 
  }
  
  return 0 ;
}

void open_file(FILE **wwf, char *argv)
{ 
  if((*wwf = fopen(argv, "r")) == NULL){
    fprintf(stderr, "\nERROR: Cannot open file: %s.\n" \
    "Please try again.\n\n", argv) ; 
    exit(EXIT_FAILURE) ; 
  }
  
  else{
    printf("\nFile: %s has been successfully opened.\n", \
    argv) ;      
  }
   
  return ; 
}

void check_file_dimensions(FILE *wwf, char *argv)
{
  int row_count = 0, char_count = 0, column_count = 0 ;
  char c = '\0' ; 
  
  while(c != EOF){
    /* Count the number of rows in file. */
    if((c = getc(wwf)) == '\n'){
      row_count++ ; 
    }
    
    /* Count characters in file, exclude EOF and newline. */
    if((c != EOF) && (c != '\n')){
      char_count++ ; 
    }
  }
      
  if(row_count != YS){
    fprintf(stderr, "\nERROR: File: %s has %d rows.\nThe " \
    "file must have %d rows.\n", argv, row_count, YS) ; 
    close_file(&wwf) ;   
    exit(EXIT_FAILURE) ;
  }
  
  /* Calculate number of columns. */  
  column_count = char_count / row_count ; 
  
  if(column_count != XS){
    fprintf(stderr, "\nERROR: File: %s has %d rows.\n" \
    "However, each row does not have %d full columns.\n" \
    "Please note the file must be %dx%d.\n", argv, YS, \
    XS, YS, XS) ; 
    close_file(&wwf) ;   
    exit(EXIT_FAILURE) ;
  }

  /* Sets file position indicator to beginning of file. */ 
  if(fseek(wwf, 0, SEEK_SET) != 0){
    fprintf(stderr, "\nERROR: File position has not been " \
    "successfully set to the beginning of the file.\n") ; 
    close_file(&wwf) ;   
    exit(EXIT_FAILURE) ;
  }
  
  return ; 
}

/* Fill array from file, discard newline and null chars. */
void fill_and_check_array(FILE *wwf, char a[YS][XS])
{
  int i, j, unexpected_character = 0 ;
  char c = '\0' ; 
  
  for(i = 0 ; i < YS ; i++)
  {
    for(j = 0 ; j < XS ; j++){
      if(((c = getc(wwf)) != '\n') \
      || ((c = getc(wwf)) != '\0')){
        a[i][j] = c ;
      }
      
      /* Check for any unexpected/incorrect characters. */ 
      if(UNEXPECTED_CHAR(a[i][j]) != 0){
        unexpected_character++ ; 
      } 
    }
  }
  
  close_file(&wwf) ;  
  
  check_characters_correct(unexpected_character) ; 
  
  return ; 
}

/* If any unexpected chars: return amount, end program. */
void check_characters_correct(int u) 
{
  if(u != 0){
    switch(u){
      case 1 : fprintf(stderr, "ERROR: %d unexpected " \
      "character.\n\n", u) ; break ; 
      default : fprintf(stderr, "ERROR: %d unexpected " \
      "characters.\n\n", u) ; 
    }
    exit(EXIT_FAILURE) ; 
  }
  
  else{
    return ; 
  }
}

/* Displays state, generates next, displays, and so on... */
void wireworld_iterations(char a[YS][XS], char b[YS][XS])
{
  int i = 0 ;

  /* Division by 2, as each cycle displays two states. */
  while(i < GENERATIONS / 2){
    print_to_screen(a) ; 
    rules(a, b) ; 
    print_to_screen(b) ; 
    rules(b, a) ; 
    i++ ; 
  } 
  
  return ; 
}

void print_to_screen(char a[YS][XS])
{
  int i, j ; 

  for(i = 0 ; i < YS ; i++){
    for(j = 0 ; j < XS ; j++){
      printf("%c", a[i][j]) ; 
    }
    printf("\n") ; 
  }
  
  return ; 
}

/* Generates next state by applying rules to the current. */
void rules(char a[YS][XS], char b[YS][XS])
{
  int i, j ; 
  unsigned short unexpected_count = 0 ;  
  
  for(i = 0 ; i < YS ; i++){
    for(j = 0 ; j < XS ; j++){
      switch(a[i][j]){
        case EMPTY : b[i][j] = EMPTY ; break ; 
        case HEAD : b[i][j] = TAIL ; break ;
        case TAIL : b[i][j] = COPPER ; break ;
        case COPPER : 
        copper_bounds_check(&a[i][j], &b[i][j], i, j) ; 
        break ;
        
        /* Default used to catch any unexpected chars. */
        default : b[i][j] = a[i][j] ; unexpected_count++ ;
      }
    }
  }
  
  check_characters_correct(unexpected_count) ; 
  
  return ; 
}

/* Prevents searching out-of-bounds when in edge columns. */
void copper_bounds_check(char *p, char *q, int i, int j)
{
  int start_position = 0, finish_position = MOORE_N ; 

  /* In first column, start search from north cell. */ 
  if(j == FIRST_COLUMN){
    start_position = SKIP_ALL_WEST_CELLS ; 
  }

  /* In last column, finish search at south cell. */ 
  if(j == LAST_COLUMN){
    finish_position = SKIP_ALL_EAST_CELLS ; 
  }
  
  copper_next(p, q, i, j, start_position, finish_position) ;
  
  return ; 
}

/* Determine next copper state by searching neighbourhood.*/ 
void copper_next(char *p, char *q, int i, int j, int start,\
int finish)
{
  /* Order allows us to use columns bounds from above ftn.*/ 
  int neighborhood[MOORE_N] = {NW, W, SW, N, S, NE, E, SE} ;
  int k, n = 0 ;
    
  for(k = start ; k < finish ; k++){
    /* 
     * Prevents searching out-of-bounds (ie, before or \
     * after) array when in top or bottom row. 
     */
    if(!OUT_OF_BOUNDS(i, j, *(neighborhood + k))){
      /* 
       * *p points to central cell. Use pointer arithmetic \
       * to search through surrounding, in-bounds, cells.
       */ 
      if(*(p + *(neighborhood + k)) == HEAD){
        n++ ;
      }
    }
  }

  /* No. of heads in neighbourhood determines next state. */  
  COPPER_TO_HEAD(n) ? (*q = HEAD) : (*q = COPPER) ; 

  return ; 
}

void close_file(FILE **wwf)
{
  if(fclose(*wwf) != 0){
    fprintf(stderr, "\nThe file has failed to close.\n\n") ; 
    exit(EXIT_FAILURE) ; 
  } 
    
  else{
    printf("\nThe file has been successfully closed.\n\n") ;  
  }
    
  return ; 
}


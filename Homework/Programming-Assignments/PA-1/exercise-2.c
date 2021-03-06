/*
 * FILE: exercise-2.c
 * USAGE: mpirun -n <number of processes> ./exercise-2 <n> <energy> <niterations> <px> <py> 
 * DESCRIPTION: This program is effecitively using the heat equation with 3 separate sources as inputs of heat
to dispate over a certain given region. 
 * OPTIONS: 
 * REQUIREMENTS: MPI interface
 * BUGS: N/A
 * AUTHOR: xXxSpicyBoiiixXx (Md Ali)
 * ORGANIZATION: Illinois Institute of Technology
 * VERSION: 1.0
 * CREATED: 09/30/2021
 * REVISION: N/A
 */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <mpi.h>

/* row-major order */
#define ind(i,j) ((j)*(bx+2)+(i))

/*
 * Function setup, setting up the grid, energy to be injected, etc.
 *
 * Input: int rank, rank number
 *        int proc, process number
 *        int argc, argument count
 *        int arcv, argument values
 *        int *n_ptr, pointer toward n
 *        int *px_ptr, pointer towards px
 *        int *py_ptr, pointer towards py
 *        int *final_flag, pointer towards final flag
 */
static void setup(int rank, int proc, int argc, char **argv,
           int *n_ptr, int *energy_ptr, int *niters_ptr, int *px_ptr, int *py_ptr, int *final_flag)
{
    int n, energy, niters, px, py;

    (*final_flag) = 0;

    if (argc < 6) {
        if (!rank)
            printf("usage: mpirun -n <number of processors> %s <n> <energy> <niters> <px> <py>\n", argv[0]);
        (*final_flag) = 1;
        return;
    }

    n = atoi(argv[1]);
    energy = atoi(argv[2]);
    niters = atoi(argv[3]);
    px = atoi(argv[4]);
    py = atoi(argv[5]);

    (*n_ptr) = n;
    (*energy_ptr) = energy;
    (*niters_ptr) = niters;
    (*px_ptr) = px;
    (*py_ptr) = py;
}

/*
* Function init_souruces, initalizes 3 sources from the main file.
*
* Input: int bx, x position
*        int by, y position
*        int offx, offset of x
*        int offy, offset of y
*        int n, number on the grid
*        int nsources, number of sources
*        int sources[][2], starts on second column
*        int *locnsources_ptr, pointer for sources
*        int locsouces[][2], source location
*/
static void init_sources(int bx, int by, int offx, int offy, int n,
                  const int nsources, int sources[][2], int *locnsources_ptr, int locsources[][2])
{
    int i, locnsources = 0;

    sources[0][0] = n / 2;
    sources[0][1] = n / 2;
    sources[1][0] = n / 3;
    sources[1][1] = n / 3;
    sources[2][0] = n * 4 / 5;
    sources[2][1] = n * 8 / 9;

    for (i = 0; i < nsources; ++i) {    /* determine which sources are in my patch */
        int locx = sources[i][0] - offx;
        int locy = sources[i][1] - offy;
        if (locx >= 0 && locx < bx && locy >= 0 && locy < by) {
            locsources[locnsources][0] = locx + 1;      /* offset by halo zone */
            locsources[locnsources][1] = locy + 1;      /* offset by halo zone */
            locnsources++;
        }
    }

    (*locnsources_ptr) = locnsources;
}

/*
* Function: alloc_bufs, allocates memory
*
* Input: int bx, for items in north or south
*        int by, for items in east or west
*        double **aold_ptr, old pointer array
*        double **anew_ptr, new pointer array
*        double **sbuf_ptr, send buffer pointer 
*        double **rbuf_ptr, receive buffer pointer
*/
static void alloc_bufs(int bx, int by, double **aold_ptr, double **anew_ptr,
                double **sbuf_ptr, double **rbuf_ptr)
{
    double *aold, *anew;
    double *sbuf;
    double *rbuf;

    anew = (double *) malloc((bx + 2) * (by + 2) * sizeof(double));
    aold = (double *) malloc((bx + 2) * (by + 2) * sizeof(double));

    memset(aold, 0, (bx + 2) * (by + 2) * sizeof(double));
    memset(anew, 0, (bx + 2) * (by + 2) * sizeof(double));

   sbuf = (double *) malloc(2*bx*sizeof(double) + 2*by*sizeof(double));
   rbuf = (double *) malloc(2*bx*sizeof(double) + 2*by*sizeof(double)); 

   memset(sbuf, 0, 2*bx*sizeof(double) + 2*by*sizeof(double));
   memset(rbuf, 0, 2*bx*sizeof(double) + 2*by*sizeof(double)); 

    (*aold_ptr) = aold;
    (*anew_ptr) = anew;
    (*sbuf_ptr) = sbuf;
    (*rbuf_ptr) = rbuf;
}

/*
 * Function: free_bufs, frees memory
 *
 * Input: double **aold_ptr, old pointer array
 *        double **anew_ptr, new pointer array
 *        double **sbuf_ptr, send buffer pointer 
 *        double **rbuf_ptr, receive buffer pointer
 */ 
static void free_bufs(double *aold, double *anew, double *sbuf, double *rbuf)
{
    free(aold);
    free(anew);
    free(sbuf);
    free(rbuf);
}

 /*
  * Function: pack_data, packs datat that needs to be sent
  *
  * Input: int bx, x coord
  *	   int by, y coord 
  *        double **aold_ptr, old pointer array
  *        double **sbuf_ptr, send buffer pointer 
  */ 
static void pack_data(int bx, int by, double *aold, double *sbuf)
{
    int i;
    
    for(i = 0; i < by; i++) 
	    sbuf[i] = aold[ind(1, i+1)];
    for(i = 0; i < by; i++)
	    sbuf[by + i] = aold[ind(bx, i + 1)];
    for(i = 0; i < bx; i++)
	    sbuf[2*by + i] = aold[ind(i+1, 1)];
    for(i = 0; i < bx; i++)
	    sbuf[2*by+bx+i] = aold[ind(i+1, by)]; 
}


 /*
  * Function: unpack_data, unpacks data that is recieved 
  *
  * Input: int bx, x coord
  *	   int by, y coord
  *        double **aold_ptr, old pointer array
  *        double **rbuf_ptr, receive buffer pointer
  */ 
static void unpack_data(int bx, int by, double *aold, double *rbuf)
{
    int i;
	
    for(i = 0; i < by; i++) 
	    aold[ind(0, i+1)] = rbuf[i];
    for(i = 0; i < by; i++)
	    aold[ind(bx + 1, i + 1)] = rbuf[by+i];
    for(i = 0; i < bx; i++)
	    aold[ind(i + 1, 0)] = rbuf[2*by+i];
    for(i = 0; i < bx; i++)
	    aold[ind(i+1, by+1)] = rbuf[2*by+bx+i];
}

 /*
 * Function: update_grid, updates the heat grid for dispatation 
 *
 * Input: int bx, x coord
 * 	  int by, y coord 
 *        double **aold, old pointer array
 *        double **anew, new pointer array
 *        double **heat_ptr, heat pointer
 */ 
static void update_grid(int bx, int by, double *aold, double *anew, double *heat_ptr)
{
    int i, j;
    double heat = 0.0;

    for (i = 1; i < bx + 1; ++i) {
        for (j = 1; j < by + 1; ++j) {
            anew[ind(i, j)] =
                anew[ind(i, j)] / 2.0 + (aold[ind(i - 1, j)] + aold[ind(i + 1, j)] +
                                         aold[ind(i, j - 1)] + aold[ind(i, j + 1)]) / 4.0 / 2.0;
            heat += anew[ind(i, j)];
        }
    }

    (*heat_ptr) = heat;
}

int main(int argc, char **argv)
{
    int rank, size;
    int n, energy, niters, px, py;

    int rx, ry;
    int north, south, west, east;
    int bx, by, offx, offy;

    const int nsources = 3;
    int sources[nsources][2];
    int locnsources;
    int locsources[nsources][2];

    double startTime, endTime; ;

    double *sbuf;
    double *rbuf;
    double *aold, *anew, *tmp;

    double heat, rheat;

    int final_flag;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    setup(rank, size, argc, argv, &n, &energy, &niters, &px, &py, &final_flag);

    if (final_flag == 1) {
        MPI_Finalize();
        exit(0);
    }
    
    int pdims[2] = {0,0};
    MPI_Dims_create(size, 2, pdims); 
    pdims[0] = px;
    pdims[1] = py;

    int periods[2] = {0,0};
    MPI_Comm topComm;
    MPI_Cart_create(MPI_COMM_WORLD, 2, pdims, periods, 0, &topComm);
    
    int coords[2]; 
    rx = rank % px;
    ry = rank / px;
    coords[0] = rx; 
    coords[1] = ry; 

    MPI_Cart_coords(topComm, rank, 2, coords);
	

    MPI_Cart_shift(topComm, 0, 1, &west, &east); 
    MPI_Cart_shift(topComm, 1, 1, &north, &south);

    bx = n / px;
    by = n / py;
    offx = rx * bx;
    offy = ry * by;
	
    startTime = MPI_Wtime(); 

    init_sources(bx, by, offx, offy, n, nsources, sources, &locnsources, locsources);
    alloc_bufs(bx, by, &aold, &anew, &sbuf, &rbuf);


        for (int i = 0; i < locnsources; ++i) {
            aold[ind(locsources[i][0], locsources[i][1])] += energy;
        }
    
        pack_data(bx, by, aold, sbuf);
        	
	int counts[4] = {by, by, bx, bx};
	int displs[4] = {0, by, 2*by, 2*by+bx};
	MPI_Alltoallv(sbuf,counts, displs, MPI_DOUBLE, rbuf, counts, displs, MPI_DOUBLE, topComm);

        unpack_data(bx, by, aold, rbuf);

        update_grid(bx, by, aold, anew, &heat);

        tmp = anew;
        anew = aold;
        aold = tmp;
   	
	endTime = MPI_Wtime(); 

       free_bufs(aold, anew, sbuf, rbuf);	
	
    MPI_Allreduce(&heat, &rheat, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    if (!rank)
        printf("[%i] last heat: %f time: %f\n", rank, rheat, endTime-startTime);

    MPI_Finalize();
    return 0;
}

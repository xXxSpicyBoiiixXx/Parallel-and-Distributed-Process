/*
 * FILE: exercise-2.c
 * USAGE:
 * DESCRIPTION:
 * OPTIONS:
 * REQUIREMENTS: MPI interface
 * BUGS: N/A
 * AUTHOR: xXxSpicyBoiiixXx (Md Ali)
 * ORGANIZATION: Illinois Institute of Technology
 * VERSION: 1.0
 * CREATED: 09/30/2021
 * REVISION: N/A
 */

/*-----Original Description of program-----*/
/*
 * 2D stencil code using a nonblocking send/receive with manual packing/unpacking.
 *
 * 2D regular grid is divided into px * py blocks of grid points (px * py = # of processes.)
 * In every iteration, each process calls nonblocking operations to exchange a halo with
 * neighbors. Grid points in a halo are packed and unpacked before and after communications.
 */
/*---------------------------------------*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <mpi.h>

/* row-major order */
#define ind(i,j) ((j)*(bx+2)+(i))

/*
 * Function: Unused function according to compiler 
 */

/*static int ind_f(int i, int j, int bx)
{
    return ind(i, j);
}
*/

/* COME BACK TO CLEAN UP
 * Function setup, setting up the grid, energy to be injected, etc.
 *
 * Input: int rank, ????
 *        int proc, process number
 *        int argc, argument count
 *        int arcv, argument values
 *        int *n_ptr, pointer toward n
 *        int *px_ptr, pointer towards px
 *        int *py_ptr, pointer towards py
 *        int *final_flag, pointer towards final flag???? why
 */
static void setup(int rank, int proc, int argc, char **argv,
           int *n_ptr, int *energy_ptr, int *niters_ptr, int *px_ptr, int *py_ptr, int *final_flag)
{
    int n, energy, niters, px, py;

    (*final_flag) = 0;

    if (argc < 6) {
        if (!rank)
            printf("usage: stencil_mpi <n> <energy> <niters> <px> <py>\n");
        (*final_flag) = 1;
        return;
    }

    n = atoi(argv[1]);  /* nxn grid */
    energy = atoi(argv[2]);     /* energy to be injected per iteration */
    niters = atoi(argv[3]);     /* number of iterations */
    px = atoi(argv[4]); /* 1st dim processes */
    py = atoi(argv[5]); /* 2nd dim processes */

//    int args[3] = {n, energy, niters}; 
//    MPI_Bcast(args, 3, MPI_INT, 0, comm);

    if (px * py != proc)
        MPI_Abort(MPI_COMM_WORLD, 1);   /* abort if px or py are wrong */
    if (n % px != 0)
        MPI_Abort(MPI_COMM_WORLD, 2);   /* abort px needs to divide n */
    if (n % py != 0)
        MPI_Abort(MPI_COMM_WORLD, 3);   /* abort py needs to divide n */

    (*n_ptr) = n;
    (*energy_ptr) = energy;
    (*niters_ptr) = niters;
    (*px_ptr) = px;
    (*py_ptr) = py;
}

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

static void alloc_bufs(int bx, int by, double **aold_ptr, double **anew_ptr,
                double **sbuf_ptr, double **rbuf_ptr)
{
    double *aold, *anew;
    double *sbuf;
    double *rbuf;

    /* allocate two working arrays */
    anew = (double *) malloc((bx + 2) * (by + 2) * sizeof(double));     /* 1-wide halo zones! */
    aold = (double *) malloc((bx + 2) * (by + 2) * sizeof(double));     /* 1-wide halo zones! */

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

static void free_bufs(double *aold, double *anew, double *sbuf, double *rbuf)
{
    free(aold);
    free(anew);
    free(sbuf);
    free(rbuf);
}

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
//    int north, south, west, east;
    int bx, by, offx, offy;

    /* three heat sources */
    const int nsources = 3;
    int sources[nsources][2];
    int locnsources;            /* number of sources in my area */
    int locsources[nsources][2];        /* sources local to my rank */

  //  double t1, t2;

  //  int iter, i;

    double *sbuf;
    double *rbuf;
    double *aold, *anew, *tmp;

    double heat, rheat;

    int final_flag;

    /* initialize MPI envrionment */
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    /* argument checking and setting */
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
    MPI_Comm topocomm;
    MPI_Cart_create(MPI_COMM_WORLD, 2, pdims, periods, 0, &topocomm); 
    /* determine my coordinates (x,y) -- rank=x*a+y in the 2d processor array */
    int coords[2]; 
    rx = rank % px;
    ry = rank / px;
    coords[0] = rx; 
    coords[1] = ry; 

    MPI_Cart_coords(topocomm, rank, 2, coords);
	
    int source, north, south, east, west; 
    MPI_Cart_shift(topocomm, 0, 1, &west, &east); 
    MPI_Cart_shift(topocomm, 1, 1, &north, &south);

    /* decompose the domain */
    bx = n / px;        /* block size in x */
    by = n / py;        /* block size in y */
    offx = rx * bx;     /* offset in x */
    offy = ry * by;     /* offset in y */

    /* initialize three heat sources */
    init_sources(bx, by, offx, offy, n, nsources, sources, &locnsources, locsources);
	
    /* allocate working arrays & communication buffers */
    alloc_bufs(bx, by, &aold, &anew, &sbuf, &rbuf);

        /* refresh heat sources */
        for (int i = 0; i < locnsources; ++i) {
            aold[ind(locsources[i][0], locsources[i][1])] += energy;    /* heat source */
        }

        /* pack data */
        pack_data(bx, by, aold, sbuf);
        	
	int counts[4] = {by, by, bx, bx};
	int displs[4] = {0, by, 2*by, 2*by+bx};
	MPI_Alltoallv(sbuf,counts, displs, MPI_DOUBLE, rbuf, counts, displs, MPI_DOUBLE, topocomm);

        unpack_data(bx, by, aold, rbuf);

        /* update grid points */
        update_grid(bx, by, aold, anew, &heat);

        /* swap working arrays */
        tmp = anew;
        anew = aold;
        aold = tmp;
   
       free_bufs(aold, anew, sbuf, rbuf);	


    /* get final heat in the system */
    MPI_Allreduce(&heat, &rheat, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    if (!rank)
        printf("[%i] last heat: %f time: LATER\n", rank, rheat);

    MPI_Finalize();
    return 0;
}


      SUBROUTINE floop25_F90(N, x, y, a)
      INTEGER i, N
      REAL*4 x(N), a(N), b(N), c(N), y(N), u, v, w

      x = u*b; y = v*b + w*a + u*c
      RETURN
      END


      SUBROUTINE floop25_F90Overhead(N, x, y, a)
      INTEGER i, N
      REAL*4 x(N), a(N), b(N), c(N), y(N), u, v, w

      RETURN
      END
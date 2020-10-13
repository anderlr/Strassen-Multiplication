#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct t_pixel
{
	int r, g, b;
} pixel;


void print_matrix_2(int n, pixel **M)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%d %d %d ", M[i][j].r, M[i][j].g, M[i][j].b);
        }
        printf("\n");
    }
}

void print_matrix(FILE *fp2, int n, pixel **M)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			fprintf(fp2, "%d %d %d ", M[i][j].r, M[i][j].g, M[i][j].b);
		}
		fprintf(fp2, "\n");
	}
}

void boring_multi(int n, pixel **X, pixel **Y, pixel **Z)
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			int res_r = 0, res_g = 0, res_b = 0;
			//imagem linha i * filtro coluna j
			for (int k = 0; k < n; k++)
			{
				res_r += X[i][k].r * Y[k][j].r;
				res_g += X[i][k].g * Y[k][j].g;
				res_b += X[i][k].b * Y[k][j].b;
			}
			Z[i][j].r = res_r;
			Z[i][j].g = res_g;
			Z[i][j].b = res_b;
		}
	}
}

pixel **soma(int n, pixel **X, pixel **Y) //Faz X+Y=Z
{
	pixel **Z = (pixel **)calloc(n, sizeof(pixel *));

	for (int i = 0; i < n; i++)
		Z[i] = (pixel *)calloc(n, sizeof(pixel));

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
		{
			Z[i][j].r = X[i][j].r + Y[i][j].r;
			Z[i][j].g = X[i][j].g + Y[i][j].g;
			Z[i][j].b = X[i][j].b + Y[i][j].b;
		}

	return Z;
}

pixel **subtr(int n, pixel **X, pixel **Y) //Faz X-Y=Z
{
	pixel **Z = (pixel **)calloc(n, sizeof(pixel *));

	for (int i = 0; i < n; i++)
		Z[i] = (pixel *)calloc(n, sizeof(pixel));

	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
		{
			Z[i][j].r = X[i][j].r - Y[i][j].r;
			Z[i][j].g = X[i][j].g - Y[i][j].g;
			Z[i][j].b = X[i][j].b - Y[i][j].b;
		}

	return Z;
}

void separa_quadrante(int n, pixel **X, pixel **X1, pixel **X2, pixel **X3, pixel **X4) // Divide a matriz X em quadrantes X1,X2,X3,X4
{

	for (int i = 0; i < n / 2; i++)
		for (int j = 0; j < n / 2; j++)
		{
			X1[i][j].r = X[i][j].r;
			X1[i][j].g = X[i][j].g;
			X1[i][j].b = X[i][j].b;

			X2[i][j].r = X[i][n / 2 + j].r;
			X2[i][j].g = X[i][n / 2 + j].g;
			X2[i][j].b = X[i][n / 2 + j].b;

			X3[i][j].r = X[n / 2 + i][j].r;
			X3[i][j].g = X[n / 2 + i][j].g;
			X3[i][j].b = X[n / 2 + i][j].b;

			X4[i][j].r = X[n / 2 + i][n / 2 + j].r;
			X4[i][j].g = X[n / 2 + i][n / 2 + j].g;
			X4[i][j].b = X[n / 2 + i][n / 2 + j].b;
		}
}

void junta_quadrante(int n, pixel **X, pixel **X1, pixel **X2, pixel **X3, pixel **X4) // Separa a matriz X em quadrantes X1,X2,X3,X4
{
	for (int i = 0; i < n / 2; i++)
		for (int j = 0; j < n / 2; j++)
		{
			X[i][j].r = X1[i][j].r;
			X[i][j].g = X1[i][j].g;
			X[i][j].b = X1[i][j].b;

			X[i][n / 2 + j].r = X2[i][j].r;
			X[i][n / 2 + j].g = X2[i][j].g;
			X[i][n / 2 + j].b = X2[i][j].b;

			X[n / 2 + i][j].r = X3[i][j].r;
			X[n / 2 + i][j].g = X3[i][j].g;
			X[n / 2 + i][j].b = X3[i][j].b;

			X[n / 2 + i][n / 2 + j].r = X4[i][j].r;
			X[n / 2 + i][n / 2 + j].g = X4[i][j].g;
			X[n / 2 + i][n / 2 + j].b = X4[i][j].b;
		}
}

pixel **strassen_multi(int n, pixel **imagem, pixel **filtro)
{
	int m = n / 2;

	//criando matriz para o resultado
	pixel **result = (pixel **)calloc(n, sizeof(pixel *));

	for (int i = 0; i < n; i++)
		result[i] = (pixel *)calloc(n, sizeof(pixel));

	if (n <= 32)
	{
		boring_multi(n, imagem, filtro, result);
		return result;
	}
	else
	{

		//criamos os 4 quadrantes da imagem
		pixel **a = (pixel **)calloc(m, sizeof(pixel *)); //quadrante 1
		pixel **b = (pixel **)calloc(m, sizeof(pixel *)); //quadrante 2
		pixel **c = (pixel **)calloc(m, sizeof(pixel *)); //quadrante 3
		pixel **d = (pixel **)calloc(m, sizeof(pixel *)); //quadrante 4

		for (int i = 0; i < m; i++)
		{
			a[i] = (pixel *)calloc(m, sizeof(pixel));
			b[i] = (pixel *)calloc(m, sizeof(pixel));
			c[i] = (pixel *)calloc(m, sizeof(pixel));
			d[i] = (pixel *)calloc(m, sizeof(pixel));
		}

		//criamos os 4 quadrantes do filtro
		pixel **e = (pixel **)calloc(m, sizeof(pixel *)); //quadrante 1
		pixel **f = (pixel **)calloc(m, sizeof(pixel *)); //quadrante 2
		pixel **g = (pixel **)calloc(m, sizeof(pixel *)); //quadrante 3
		pixel **h = (pixel **)calloc(m, sizeof(pixel *)); //quadrante 4

		for (int i = 0; i < m; i++)
		{
			e[i] = (pixel *)calloc(m, sizeof(pixel));
			f[i] = (pixel *)calloc(m, sizeof(pixel));
			g[i] = (pixel *)calloc(m, sizeof(pixel));
			h[i] = (pixel *)calloc(m, sizeof(pixel));
		}

		separa_quadrante(n, imagem, a, b, c, d); //preechendo os quadrantes da imagem
		separa_quadrante(n, filtro, e, f, g, h); //preechendo os quadrantes do filtro

		//criando as matrizes r,s,t,u para armazenar o resultado das operações
		pixel **r = (pixel **)calloc(m, sizeof(pixel *));
		pixel **s = (pixel **)calloc(m, sizeof(pixel *));
		pixel **t = (pixel **)calloc(m, sizeof(pixel *));
		pixel **u = (pixel **)calloc(m, sizeof(pixel *));
		for (int i = 0; i < m; i++)
		{
			r[i] = (pixel *)calloc(m, sizeof(pixel));
			s[i] = (pixel *)calloc(m, sizeof(pixel));
			t[i] = (pixel *)calloc(m, sizeof(pixel));
			u[i] = (pixel *)calloc(m, sizeof(pixel));
		}

		//criando as matrizes para armazenar os resultados das chamadas p1 até p7
		pixel **p1 = (pixel **)calloc(m, sizeof(pixel *));
		pixel **p2 = (pixel **)calloc(m, sizeof(pixel *));
		pixel **p3 = (pixel **)calloc(m, sizeof(pixel *));
		pixel **p4 = (pixel **)calloc(m, sizeof(pixel *));
		pixel **p5 = (pixel **)calloc(m, sizeof(pixel *));
		pixel **p6 = (pixel **)calloc(m, sizeof(pixel *));
		pixel **p7 = (pixel **)calloc(m, sizeof(pixel *));
		for (int i = 0; i < m; i++)
		{
			p1[i] = (pixel *)calloc(m, sizeof(pixel));
			p2[i] = (pixel *)calloc(m, sizeof(pixel));
			p3[i] = (pixel *)calloc(m, sizeof(pixel));
			p4[i] = (pixel *)calloc(m, sizeof(pixel));
			p5[i] = (pixel *)calloc(m, sizeof(pixel));
			p6[i] = (pixel *)calloc(m, sizeof(pixel));
			p7[i] = (pixel *)calloc(m, sizeof(pixel));
		}

		p1 = strassen_multi(m, a, subtr(m, f, h));			   //calculo de p1  OK
		p2 = strassen_multi(m, soma(m, a, b),h);			   //calculo de p2  OK
		p3 = strassen_multi(m, soma(m, c, d),e);			   //calculo de p3	OK
		p4 = strassen_multi(m, d, subtr(m, g, e));			   //calculo de p4	OK
		p5 = strassen_multi(m, soma(m, a, d), soma(m, e, h));  //calculo de p5	OK
		p6 = strassen_multi(m, subtr(m, b, d), soma(m, g, h)); //calculo de p6	OK
		p7 = strassen_multi(m, subtr(m, a, c), soma(m, e, f)); //calculo de p7	OK
		//junta
		r = soma(m, subtr(m, p6, p2), soma(m, p5, p4));
		s = soma(m, p1, p2);
		t = soma(m, p3, p4);
		u = soma(m, subtr(m, p5, p3), subtr(m, p1, p7));

		junta_quadrante(n, result, r, s, t, u);
		//retorna o resultado

		return result;
	}

	//p1= a(f-h)
	//p2= h(a+b)
	//p3= e(c+d)
	//p4= d(g-e)
	//p5= (a+d)(e+h)
	//p6= (b-d)(g+h)
	//p7= (a-c)(e+f)

	//r = p5+p4-p2+p6
	//s = p1+p2
	//t = p3+p4
	//u = p5+p1-p3-p7
}

int main(int argc, char *argv[])
{
	int n, aux_i;
	char aux;

	pixel **imagem;
	pixel **filtro;
	pixel **resultado;

	FILE *fp;
	FILE *fp2;

	fp = fopen("04.in", "r");

	if (fp == 0)
	{
		printf("Unable to open file");
		return 1;
	}

	fp2 = fopen("resp_04.ppm", "wp+");
	if (fp2 == 0)
	{
		printf("Unable to open file");
		return 1;
	}

	fscanf(fp, "%c%d \n", &aux, &aux_i);
	//getchar();
	fscanf(fp, "%d %d", &n, &aux_i);
	fscanf(fp, "%d", &aux_i);

	//Vou considerar que imagem[i][j] é o pixel da linha i e coluna j
	//note que essa convenção é arbitrária mas uma vez que eu assumi isso
	//devo tomar cuidado para não trocar
	imagem = (pixel **)calloc(n,sizeof(pixel *));
	filtro = (pixel **)calloc(n,sizeof(pixel *));
	resultado = (pixel **)calloc(n,sizeof(pixel *));
	for (int i = 0; i < n; i++)
	{
		imagem[i] = (pixel *)calloc(n,sizeof(pixel));
		filtro[i] = (pixel *)calloc(n,sizeof(pixel));
		resultado[i] = (pixel *)calloc(n,sizeof(pixel));
	}

	//cuidado, aqui vou ler linha a linha
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			fscanf(fp, "%d %d %d", &(imagem[i][j].r), &(imagem[i][j].g), &(imagem[i][j].b));
		}
	}

	//lendo linha a linha do filtro
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			fscanf(fp, "%d %d %d", &(filtro[i][j].r), &(filtro[i][j].g), &(filtro[i][j].b));
		}
	}

	if (n <= 32)
	{
		boring_multi(n, imagem, filtro, resultado);
		//strassen_multi(n, imagem, filtro, resultado);
	}
	else
	{
		resultado = strassen_multi(n, imagem, filtro);
	}

	fprintf(fp2, "P3\n%d %d\n255\n", n, n);
	print_matrix(fp2, n, resultado);

	return 0;
}

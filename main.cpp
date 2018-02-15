#include <fstream>
#include <iostream>
#include <vector>
#include "simple_matrix.hpp"
using namespace std;

template<class T>
SMatrix<T> strassen(SMatrix<T> A, SMatrix<T> B){
	SMatrix<T> C(A.size());
	if(A.size()==1){

		C(0,0) = A(0,0) * B(0,0);
	
	} else {

		const int H = A.size()/2;
		int i,j;
		SMatrix<T> subMata11(H), subMata12(H), subMata21(H), subMata22(H), subMatb11(H), subMatb12(H), subMatb21(H), 
					subMatb22(H) ,subMatc11(H), subMatc12(H), subMatc21(H), subMatc22(H), p1(H), p2(H), p3(H), p4(H), 
					p5(H), p6(H), p7(H), tempResult1(H), tempResult2(H);

		createSubMat(subMata11, subMata12, subMata21, subMata22, subMatb11, subMatb12, subMatb21, 
					subMatb22, A, B, H);

		tempResult1 = sumOfMatrix(subMata11, subMata22, H);
		tempResult2 = sumOfMatrix(subMatb11, subMatb22, H);
		p1 = strassen(tempResult1, tempResult2);

		tempResult1 = sumOfMatrix(subMata21, subMata22, H);
		p2 = strassen(tempResult1, subMatb11);

		tempResult2 = subtractionOfMatrix(subMatb12, subMatb22, H);
		p3 = strassen(subMata11, tempResult2); 

		tempResult2 = subtractionOfMatrix(subMatb21, subMatb11, H);
		p4 = strassen(subMata22, tempResult2);

		tempResult1 = sumOfMatrix(subMata11, subMata12, H);
		p5 = strassen(tempResult1, subMatb22);   

		tempResult1 = subtractionOfMatrix(subMata21, subMata11, H);
		tempResult2 = sumOfMatrix(subMatb11, subMatb12, H);
		p6 = strassen(tempResult1, tempResult2);

		tempResult1 = subtractionOfMatrix(subMata12, subMata22, H);
		tempResult2 = sumOfMatrix(subMatb21, subMatb22, H);
		p7 = strassen(tempResult1, tempResult2);

        subMatc12 = sumOfMatrix(p3, p5, H);
        subMatc21 = sumOfMatrix(p2, p4, H);

        tempResult1 = sumOfMatrix(p1, p4, H);
        tempResult2 = sumOfMatrix(tempResult1, p7, H);
        subMatc11 = subtractionOfMatrix(tempResult2, p5, H);

        tempResult1 = sumOfMatrix(p1, p3, H);
        tempResult2 = sumOfMatrix(tempResult1, p6, H);
        subMatc22 = subtractionOfMatrix(tempResult2, p2, H);

	        
	 	for (i = 0; i < H ; i++) {
	 		for (j = 0 ; j < H ; j++) {
	 			C(i,j) = subMatc11(i,j);
	 			C(i, j+H) = subMatc12(i,j);
	 			C(i+H, j) = subMatc21(i,j);
	 			C(i+H, j+H) = subMatc22(i,j);
	 		}
		}
	}
	    
	return C;    
}

template<class T>
void createSubMat(SMatrix<T> &subMata11, SMatrix<T> &subMata12, SMatrix<T> &subMata21, 
				 SMatrix<T> &subMata22, SMatrix<T> &subMatb11, SMatrix<T> &subMatb12, SMatrix<T> &subMatb21, 
				 SMatrix<T> &subMatb22, SMatrix<T> A, SMatrix<T> B, int H) 
{
	for (int i = 0; i < H; i++) {
			for (int j = 0; j < H; j++) {
				subMata11(i,j) = A(i,j);
				subMata12(i,j) = A(i,j + H);
				subMata21(i,j) = A(i+H,j);
				subMata22(i,j) = A(i+H, j+H);

				subMatb11(i,j) = B(i,j);
				subMatb12(i,j) = B(i,j + H);
				subMatb21(i,j) = B(i+H,j);
				subMatb22(i,j) = B(i+H, j+H);
			}
		}
}

template<class T>
SMatrix<T> sumOfMatrix(SMatrix<T> A, SMatrix<T> B, int H) {
	SMatrix<T> C(A.size());
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < H; j++) {
			C(i,j) = A(i,j) + B(i,j);
		}
	}
	return C;
}

template<class T>
SMatrix<T> subtractionOfMatrix(SMatrix<T> A, SMatrix<T> B, int H) {
	SMatrix<T> C(A.size());
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < H; j++) {
			C(i,j) = A(i,j) - B(i,j);
		}
	}
	return C; 
}

int main(int argc, char* argv[]){
	 
	ifstream in(argv[1]);

	if(!in){
		cout<<"Cannot open  input file;"<<endl;
		return -1;
	}

	ofstream out(argv[2]);

	if(!out){
		cout<<"Cannot open  output file;"<<endl;
		return -1;
	}
	int datasets;
	in>>datasets;    
	    
	for(int i=0;i<datasets;i++)
	{
		int N;
		in>>N;
		//cout << N << endl;


		SMatrix<float> A(N), B(N), C(N);


		for(int j=0;j<N;j++)
		{
			for(int k=0;k<N;k++)
			{
				in>>A(j,k);
				//cout << A(j,k) << endl;
			}
		}
		for(int j=0;j<N;j++)
		{
			for(int k=0;k<N;k++)
			{
				in>>B(j,k);
			}
		}

		C = strassen(A, B);


		for(int i=0;i<N;++i){
			for(int j=0;j<N;++j)
			{
				out<<C(i,j)<<" ";
			}
		}

		out<<endl;
	}
	    
   return 0;
}    
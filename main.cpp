#include <iostream>
#include <ctime>
#include "MyMatrix.h"
#include "Generator.h"
#include "Approximation.h"
#include "MyFunctions.h"
/*
    Author: Dawid Lipiński
    Created: 11.01.2018
    Modified: 14.01.2018
    Description: Program wykonuje pomiary czasów wykonywania się obliczeń macierzy metodami:
                    -Gaussa
                    -Gaussa zoptymalizowanego
                    -Siedla
                    -SpraseMatrix
                 Nastepnie za pomocą aproksymacji średniokwadratowej dyskretnej oraz danych zebranych wcześniej, tworzy wielomiany aproksymacyjne.
                 Umożliwiają one obliczenie czasu wykonywania działania daną metodą na podstawie danego rozmiaru macierzy.
*/


typedef std::numeric_limits< double > dbl;

using namespace std;


int main()
{
    int sizes_amount=8;
    vector<int> board_sizes(sizes_amount,0);
    board_sizes = {392,512,648,800,964,1144,1552,2024};

    vector<double> equations_sizes(sizes_amount,0);
    vector<double> generating_time(sizes_amount,0);

    vector<double> gauss_partial_times(sizes_amount,0);
    vector<double> gauss_partial_optimized_times(sizes_amount,0);
    vector<double> eigen_times(sizes_amount,0);
    vector<double> seidel_times(sizes_amount,0);
    vector<double> seidel_eigen_times(sizes_amount,0);

    ofstream outputFile("times_data.txt");
    outputFile << "Rozmiar;Generowanie;Gauss;Gauss Opt;Eigen;Seidel;Siedel Eigen" << endl;

    for(int i=0;i<sizes_amount;i++){
        cout << "Counting board size "<<board_sizes[i]<<"..."<<endl;

        clock_t begin = clock();
        vector < vector <double > > matrix = generate_matrix(board_sizes[i]);
        clock_t end = clock();
        double generator_time =  double(end - begin) / CLOCKS_PER_SEC;
        equations_sizes[i] = matrix.size();
        generating_time[i] = generator_time;
        MyMatrix generated_matrix(matrix.size(),matrix.size()+1);
        generated_matrix = matrix;

        double gauss_partial_time = generated_matrix.Gauss_partial_time();
        double gauss_partial_optimized_time = generated_matrix.Gauss_partial_optimized_time();
        double eigen_time = generated_matrix.Eigen_time();
        double seidel_time = generated_matrix.Siedel_time();
        double seidel_eigen_time = generated_matrix.Siedel_Eigen_time();

        outputFile << matrix.size() << ";" << generator_time << ";" << gauss_partial_time << ";" << gauss_partial_optimized_time << ";" << eigen_time << ";" << seidel_time <<";" <<seidel_eigen_time<< endl;

        gauss_partial_times[i]=gauss_partial_time;
        gauss_partial_optimized_times[i]=gauss_partial_optimized_time;
        eigen_times[i]=eigen_time;
        seidel_times[i]=seidel_time;
        seidel_eigen_times[i]=seidel_eigen_time;

    }
    outputFile.close();


    Approximation gauss_partial_polynomial(equations_sizes,gauss_partial_times,3);
    Approximation gauss_partial_optimized_polynomial(equations_sizes,gauss_partial_optimized_times,2);
    Approximation eigen_polynomial(equations_sizes,eigen_times,1);
    Approximation siedel_polynomial(equations_sizes,seidel_times,2);
    Approximation siedel_eigen_polynomial(equations_sizes,seidel_eigen_times,1);

    gauss_partial_polynomial.create_polynominal();
    gauss_partial_optimized_polynomial.create_polynominal();
    eigen_polynomial.create_polynominal();
    siedel_polynomial.create_polynominal();
    siedel_eigen_polynomial.create_polynominal();

    ofstream outputFile1("equations.txt");
    outputFile1 << gauss_partial_polynomial.to_string("Gauss partial") << endl;;
    outputFile1 << gauss_partial_optimized_polynomial.to_string("Gauss partial optimized") << endl;
    outputFile1 << eigen_polynomial.to_string("Eigen") << endl;
    outputFile1 << siedel_polynomial.to_string("Siedel") << endl;
    outputFile1 << siedel_eigen_polynomial.to_string("Siedel Eigen") << endl;
    outputFile1.close();

    ofstream outputFile3("calculations_check.txt");
    outputFile3 << "Rozmiar;Gauss;Gauss Opt; Eigen; Siedel;Siedel Eigen"<<endl;
    for(int i=0;i<sizes_amount;i++){
        outputFile3 << equations_sizes[i]<<";" <<abs(gauss_partial_times[i]- gauss_partial_polynomial.solve(equations_sizes[i])) << ";";
        outputFile3 << abs(gauss_partial_optimized_times[i]- gauss_partial_optimized_polynomial.solve(equations_sizes[i])) << ";";
        outputFile3 << abs(eigen_times[i]- eigen_polynomial.solve(equations_sizes[i])) << ";";
        outputFile3 << abs(seidel_times[i]- siedel_polynomial.solve(equations_sizes[i])) << ";"<<endl;
        outputFile3 << abs(seidel_times[i]- siedel_eigen_polynomial.solve(equations_sizes[i])) << ";"<<endl;
    }
    outputFile3.close();

    ofstream outputFile2("100000_times.txt");
    outputFile2 << gauss_partial_polynomial.solve(100000) << endl;
    outputFile2 << gauss_partial_optimized_polynomial.solve(100000) << endl;
    outputFile2 << eigen_polynomial.solve(100000) << endl;
    outputFile2 << siedel_polynomial.solve(100000) << endl;
    outputFile2 << siedel_eigen_polynomial.solve(100000) << endl;
    outputFile2.close();

    cout << "generating big matrix" << endl;
    ofstream outputFile4("large_matrix.txt");

    double k100_time= eigen_100000_time();



    cout << "solved time: " <<k100_time<<endl;
    outputFile4 << "solved time: " << k100_time<< endl;

    return 0;

}




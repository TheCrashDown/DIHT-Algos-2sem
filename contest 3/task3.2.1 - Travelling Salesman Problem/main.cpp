#include "headers/DotsGenerator.h"
#include "headers/MSTCreator.h"
#include "headers/AdvancedIGraph.h"
#include "headers/MatrixGraph.h"
#include "headers/BrutForce.h"
#include "headers/ProblemSolver.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>


int main()
{

    std::ofstream fout;
    fout.open("res.log");

    fout << "Execution report\n\n\n";

    double avg_brutforce = 0.0;
    double avg_mst = 0.0;
    double avg_ratio = 0.0;
    double avg_error = 0.0;
    double avg_sqrt_err = 0.0;
    int test_counter = 0;
    int ok_counter = 0;
    int fail_counter = 0;

    //  запускаем тесты для всех количесив точек на отрезке 2..10
    for(int n = 2; n <= 10; ++n){

        fout << "\n================== Verticies number: " << n << " ======================\n\n";

        //  запускаем по 5 тестов для каждого количества точек
        for(int i = 0; i < 5; ++i){

            DotsGenerator gen(100, 100, n);
            std::vector< std::pair< int, int > > verticies;
            gen.getDots(verticies);

            std::string s = "dots " + std::to_string(n) + " " + std::to_string(i);
            std::ofstream dots(s);
            for(int i = 0; i < n; ++i){
                dots << verticies[i].first << " " << verticies[i].second << "\n";
            }


            AdvancedIGraph* graph = new MatrixGraph(n);

            for(int i = 0; i < n; ++i){
                for(int j = i + 1; j < n; ++j){
                    double x = (verticies[i].first - verticies[j].first)*(verticies[i].first - verticies[j].first);
                    double y = (verticies[i].second - verticies[j].second)*(verticies[i].second - verticies[j].second);
                    double weight = std::sqrt(x + y);
                    graph->setEdge(i, j, weight);
                    graph->setEdge(j, i, weight);
                }
            }

            BrutForce brutForce(*graph);

            ProblemSolver problemSolver(*graph);

            double result_brut = brutForce.findMinPath();
            double result_mst = problemSolver.findMinPath();

            avg_brutforce += result_brut;
            avg_mst += result_mst;
            avg_ratio +=  result_mst / result_brut;
            avg_error += (result_mst - result_brut);
            avg_sqrt_err += (result_brut - result_mst)*(result_brut - result_mst);
            test_counter++;
            fout << "Optimal solution:     " << result_brut << "\n";
            fout << "MST solution:         " << result_mst << "\n";
            fout << "MST / Opt ratio:      " << result_mst / result_brut;
            if(result_mst / result_brut < 2){
                fout << " -- OK\n";
                ok_counter++;
            } else {
                fout << " -- FAIL\n";
                fail_counter++;
            }
            fout << "Std error:            " << (result_mst - result_brut) << "\n";
            fout << "Sqrt error:           " << (result_brut - result_mst)*(result_brut - result_mst) << "\n";

            fout << "\n";
        }

    }

    fout << "\n\n========================== Summary: ==========================\n\n\n";

    fout << "Tests passed :            " << test_counter << "\n";
    fout << "      OK : " << ok_counter << ", Failed : " << fail_counter << "\n";
    fout << "Average optimal result:   " << avg_brutforce / test_counter << "\n";
    fout << "Average mst result:       " << avg_mst / test_counter << "\n";
    fout << "Average MST / Opt ratio:  " << avg_ratio / test_counter << "\n";
    fout << "Average std error:        " << avg_error / test_counter << "\n";
    fout << "Average sqrt error:       " << sqrt(avg_sqrt_err / test_counter) << "\n";





    return 0;
}

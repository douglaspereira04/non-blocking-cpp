#include <matplot/matplot.h>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string> 
#include <string_view> 
#include <numeric>
#include <vector>
#include <regex>



double average(std::vector<double> const& v){
    if(v.empty()){
        return 0;
    }
    long double sum = 0;
    for (auto &&i : v)
    {
        sum += i;
    }
    
    return sum *1.0/ v.size();
}

std::string escaped_underline(std::string str){
  return std::regex_replace(str, std::regex("_"), " ");
}

std::vector<std::string> scalability = {
    "scalability_empty_big.csv",
    "scalability_empty_small.csv",
    "scalability_filled_big.csv",
    "scalability_filled_small.csv"
};

std::vector<std::string> pc = {
    "pc_empty_big.csv",
    "pc_empty_small.csv",
    "pc_filled_big.csv",
    "pc_filled_small.csv"
};


void plot_files(std::vector<std::string> files, std::string image_name, std::string folder, std::vector<std::string> xticklabels){
    std::unordered_map<std::string, std::vector<std::vector<double>>> map;
    std::unordered_map<std::string, std::vector<double>> avg_map;

    for (auto &&file_name : files)
    {
        std::cout << file_name << std::endl;
        std::ifstream infile(file_name);
        std::string line;
        getline( infile, line );
        while(getline( infile, line )){
            std::string temp;
            std::istringstream  ss(line);

            getline(ss, temp, ','); 
            std::string name = temp;

            getline(ss, temp, ',');
            int threads = std::stoi(temp);

            getline(ss, temp, ',');
            double elapsed_time = std::stol(temp)/1000000000.0;


            if(map.find(name) == map.end()){
                std::vector<std::vector<double>> v(5);
                map.emplace(name, v);

                std::vector<double> avgv(5,0.0);
                avg_map.emplace(name, avgv);
            }

            int pos = ((int)log2(threads))-1;
            if(pos == -1){
                pos = 0;
            }

            map[name][pos].push_back(elapsed_time);
        }

        for (const auto& kv : map) {
            int i = 0;
            for (auto list : kv.second){
                double avg;

                avg = average(list);
                avg_map[kv.first][i] = avg;
                i++;
            }

        }
    }
    std::vector<std::string> legend_vector;

    std::vector<double> x = {1,2,3,4,5};

    std::string line_spec[] = {"-o", "-x", "-+", "-v", "-^", "-*", "-s", "-d"};
    int i = 0;
    for (const auto& kv : avg_map) {
        std::string_view spec = line_spec[i];
        matplot::plot(x, kv.second, spec);
        i = (i+1)%8;
        matplot::hold(matplot::on);
        std::string name = escaped_underline(kv.first);
        std::cout << name << std::endl;
        legend_vector.push_back(name);
    }

    matplot::xticks({0,1,2,3,4,5,6});
    matplot::xticklabels(xticklabels);
    matplot::xrange({0.9,5.1});
    
    matplot::grid(matplot::on);
    matplot::xlabel("Threads");
    matplot::ylabel("Tempo de execução (s)");
    auto lgd = matplot::legend(legend_vector);
    lgd->inside(false);
    lgd->location(matplot::legend::general_alignment::right);
    std::string path = folder+image_name+".png";
    matplot::title("");
    matplot::save(path);
    matplot::hold(false);
}

void plot_file(std::string file, std::string folder, std::vector<std::string> xticklabels){
    std::vector<std::string> v = {"results/queue/"+file};
    plot_files(v, file, folder, xticklabels);
}

void plot_all(std::string folder){
    for (auto &&file : scalability)
    {
        plot_file(file, folder, {" ","1","4","8", "16", "32" " "});
    }
    for (auto &&file : pc)
    {
        plot_file(file, folder, {" ","2","4","8", "16", "32" " "});
    }

}
int main(int argc, char *argv[]){
    plot_all("results/queue/");
    plot_all("results/queue/");
    return 0;
}
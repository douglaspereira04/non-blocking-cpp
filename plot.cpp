#include <matplot/matplot.h>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string> 
#include <numeric>
#include <vector>

double average(std::vector<long> const& v){
    if(v.empty()){
        return 0;
    }
    long long sum = 0;
    for (auto &&i : v)
    {
        sum += i;
    }
    
    return sum *1.0/ v.size();
}

std::vector<std::string> dist_a = {
"vazio_poucas_pequenas_a.csv",
"vazio_muitas_pequenas_a.csv",
"cheio_poucas_pequenas_a.csv",
"cheio_muitas_pequenas_a.csv",
"vazio_poucas_grandes_a.csv",
"vazio_muitas_grandes_a.csv",
"cheio_poucas_grandes_a.csv",
"cheio_muitas_grandes_a.csv"
};


std::vector<std::string> dist_a_big = {
    "vazio_poucas_grandes_a.csv",
    "vazio_muitas_grandes_a.csv",
    "cheio_poucas_grandes_a.csv",
    "cheio_muitas_grandes_a.csv"
};

std::vector<std::string> dist_b_big = {
    "vazio_poucas_grandes_b.csv",
    "vazio_muitas_grandes_b.csv",
    "cheio_poucas_grandes_b.csv",
    "cheio_muitas_grandes_b.csv"
};

std::vector<std::string> dist_c_big = {
    "vazio_poucas_grandes_c.csv",
    "vazio_muitas_grandes_c.csv",
    "cheio_poucas_grandes_c.csv",
    "cheio_muitas_grandes_c.csv"
};


std::vector<std::string> dist_a_small = {
"vazio_poucas_pequenas_a.csv",
"vazio_muitas_pequenas_a.csv",
"cheio_poucas_pequenas_a.csv",
"cheio_muitas_pequenas_a.csv"
};

std::vector<std::string> dist_a_full = {
    "cheio_poucas_pequenas_a.csv",
    "cheio_poucas_grandes_a.csv",
    "cheio_muitas_pequenas_a.csv",
    "cheio_muitas_grandes_a.csv"
};

std::vector<std::string> dist_a_many = {
"vazio_muitas_pequenas_a.csv",
"cheio_muitas_pequenas_a.csv",
"vazio_muitas_grandes_a.csv",
"cheio_muitas_grandes_a.csv"
};

std::vector<std::string> dist_a_few  = {
"vazio_poucas_pequenas_a.csv",
"cheio_poucas_pequenas_a.csv",
"vazio_poucas_grandes_a.csv",
"cheio_poucas_grandes_a.csv"
};

std::vector<std::string> dist_a_few_small  = {
"vazio_poucas_pequenas_a.csv",
"cheio_poucas_pequenas_a.csv"
};

std::vector<std::string> dist_b_few_small  = {
"vazio_poucas_pequenas_b.csv",
"cheio_poucas_pequenas_b.csv"
};

std::vector<std::string> dist_c_few_small  = {
"vazio_poucas_pequenas_c.csv",
"cheio_poucas_pequenas_c.csv"
};

std::vector<std::string> dist_b = {

"vazio_poucas_pequenas_b.csv",
"vazio_muitas_pequenas_b.csv",
"cheio_poucas_pequenas_b.csv",
"cheio_muitas_pequenas_b.csv",
"vazio_poucas_grandes_b.csv",
"vazio_muitas_grandes_b.csv",
"cheio_poucas_grandes_b.csv",
"cheio_muitas_grandes_b.csv"
};

std::vector<std::string> dist_b_small = {

"vazio_poucas_pequenas_b.csv",
"vazio_muitas_pequenas_b.csv",
"cheio_poucas_pequenas_b.csv",
"cheio_muitas_pequenas_b.csv"
};


std::vector<std::string> dist_b_many = {
"vazio_muitas_pequenas_b.csv",
"cheio_muitas_pequenas_b.csv",
"vazio_muitas_grandes_b.csv",
"cheio_muitas_grandes_b.csv"
};

std::vector<std::string> dist_b_few  = {
"vazio_poucas_pequenas_b.csv",
"cheio_poucas_pequenas_b.csv",
"vazio_poucas_grandes_b.csv",
"cheio_poucas_grandes_b.csv"
};

std::vector<std::string> dist_c = {
"vazio_poucas_pequenas_c.csv",
"vazio_muitas_pequenas_c.csv",
"cheio_poucas_pequenas_c.csv",
"cheio_muitas_pequenas_c.csv",
"vazio_poucas_grandes_c.csv",
"vazio_muitas_grandes_c.csv",
"cheio_poucas_grandes_c.csv",
"cheio_muitas_grandes_c.csv"
};

std::vector<std::string> dist_c_small = {
"vazio_poucas_pequenas_c.csv",
"vazio_muitas_pequenas_c.csv",
"cheio_poucas_pequenas_c.csv",
"cheio_muitas_pequenas_c.csv"
};

std::vector<std::string> dist_b_c_many_big = {
    "vazio_muitas_pequenas_c.csv",
    "cheio_muitas_pequenas_c.csv",
    "vazio_muitas_grandes_c.csv",
    "cheio_muitas_grandes_c.csv"
};

std::vector<std::string> dist_c_many = {
"vazio_muitas_grandes_c.csv",
"cheio_muitas_grandes_c.csv"
"vazio_muitas_grandes_b.csv",
"cheio_muitas_grandes_b.csv"
};

std::vector<std::string> dist_c_few  = {
"vazio_poucas_pequenas_c.csv",
"cheio_poucas_pequenas_c.csv",
"vazio_poucas_grandes_c.csv",
"cheio_poucas_grandes_c.csv"
};

std::vector<std::string> small = {
"vazio_poucas_pequenas_a.csv",
"vazio_muitas_pequenas_a.csv",
"cheio_poucas_pequenas_a.csv",
"cheio_muitas_pequenas_a.csv",
"vazio_poucas_pequenas_b.csv",
"vazio_muitas_pequenas_b.csv",
"cheio_poucas_pequenas_b.csv",
"cheio_muitas_pequenas_b.csv",
"vazio_poucas_pequenas_c.csv",
"vazio_muitas_pequenas_c.csv",
"cheio_poucas_pequenas_c.csv",
"cheio_muitas_pequenas_c.csv"
};


std::vector<std::string> big = {
"vazio_poucas_grandes_a.csv",
"vazio_muitas_grandes_a.csv",
"cheio_poucas_grandes_a.csv",
"cheio_muitas_grandes_a.csv",
"vazio_poucas_grandes_b.csv",
"vazio_muitas_grandes_b.csv",
"cheio_poucas_grandes_b.csv",
"cheio_muitas_grandes_b.csv",
"vazio_poucas_grandes_c.csv",
"vazio_muitas_grandes_c.csv",
"cheio_poucas_grandes_c.csv",
"cheio_muitas_grandes_c.csv"
};

std::vector<std::string> empty = {
"vazio_poucas_pequenas_a.csv",
"vazio_muitas_pequenas_a.csv",
"vazio_poucas_pequenas_b.csv",
"vazio_muitas_pequenas_b.csv",
"vazio_poucas_pequenas_c.csv",
"vazio_muitas_pequenas_c.csv",
"vazio_poucas_grandes_a.csv",
"vazio_muitas_grandes_a.csv",
"vazio_poucas_grandes_b.csv",
"vazio_muitas_grandes_b.csv",
"vazio_poucas_grandes_c.csv",
"vazio_muitas_grandes_c.csv"
};


std::vector<std::string> full = {
"cheio_poucas_pequenas_a.csv",
"cheio_muitas_pequenas_a.csv",
"cheio_poucas_pequenas_b.csv",
"cheio_muitas_pequenas_b.csv",
"cheio_poucas_pequenas_c.csv",
"cheio_muitas_pequenas_c.csv",
"cheio_poucas_grandes_a.csv",
"cheio_muitas_grandes_a.csv",
"cheio_poucas_grandes_b.csv",
"cheio_muitas_grandes_b.csv",
"cheio_poucas_grandes_c.csv",
"cheio_muitas_grandes_c.csv"
};

std::vector<std::string> all = {
"cheio_muitas_pequenas_a.csv",
"cheio_muitas_pequenas_b.csv",
"cheio_muitas_pequenas_c.csv",
"cheio_muitas_grandes_a.csv",
"cheio_muitas_grandes_b.csv",
"cheio_muitas_grandes_c.csv",
"vazio_muitas_pequenas_a.csv",
"vazio_muitas_pequenas_b.csv",
"vazio_muitas_pequenas_c.csv",
"vazio_muitas_grandes_a.csv",
"vazio_muitas_grandes_b.csv",
"vazio_muitas_grandes_c.csv",
"cheio_poucas_pequenas_a.csv",
"cheio_poucas_pequenas_b.csv",
"cheio_poucas_pequenas_c.csv",
"cheio_poucas_grandes_a.csv",
"cheio_poucas_grandes_b.csv",
"cheio_poucas_grandes_c.csv",
"vazio_poucas_pequenas_a.csv",
"vazio_poucas_pequenas_b.csv",
"vazio_poucas_pequenas_c.csv",
"vazio_poucas_grandes_a.csv",
"vazio_poucas_grandes_b.csv",
"vazio_poucas_grandes_c.csv"
};

std::vector<std::string> many_big = {
"cheio_muitas_grandes_a.csv",
"cheio_muitas_grandes_b.csv",
"cheio_muitas_grandes_c.csv",
"vazio_muitas_grandes_a.csv",
"vazio_muitas_grandes_b.csv",
"vazio_muitas_grandes_c.csv"
};

std::vector<std::string> many_small = {

"cheio_muitas_pequenas_a.csv",
"cheio_muitas_pequenas_b.csv",
"cheio_muitas_pequenas_c.csv",
"vazio_muitas_pequenas_a.csv",
"vazio_muitas_pequenas_b.csv",
"vazio_muitas_pequenas_c.csv"
};

std::vector<std::string> few_small = {

"cheio_poucas_pequenas_a.csv",
"cheio_poucas_pequenas_b.csv",
"cheio_poucas_pequenas_c.csv",
"vazio_poucas_pequenas_a.csv",
"vazio_poucas_pequenas_b.csv",
"vazio_poucas_pequenas_c.csv"
};

std::vector<std::string> few_big = {

"cheio_poucas_grandes_a.csv",
"cheio_poucas_grandes_b.csv",
"cheio_poucas_grandes_c.csv",
"vazio_poucas_grandes_a.csv",
"vazio_poucas_grandes_b.csv",
"vazio_poucas_grandes_c.csv"
};

std::vector<std::string> many = {

"cheio_muitas_pequenas_a.csv",
"cheio_muitas_pequenas_b.csv",
"cheio_muitas_pequenas_c.csv",
"cheio_muitas_grandes_a.csv",
"cheio_muitas_grandes_b.csv",
"cheio_muitas_grandes_c.csv",
"vazio_muitas_pequenas_a.csv",
"vazio_muitas_pequenas_b.csv",
"vazio_muitas_pequenas_c.csv",
"vazio_muitas_grandes_a.csv",
"vazio_muitas_grandes_b.csv",
"vazio_muitas_grandes_c.csv"
};


std::vector<std::string> few = {

"cheio_poucas_pequenas_a.csv",
"cheio_poucas_pequenas_b.csv",
"cheio_poucas_pequenas_c.csv",
"cheio_poucas_grandes_a.csv",
"cheio_poucas_grandes_b.csv",
"cheio_poucas_grandes_c.csv",
"vazio_poucas_pequenas_a.csv",
"vazio_poucas_pequenas_b.csv",
"vazio_poucas_pequenas_c.csv",
"vazio_poucas_grandes_a.csv",
"vazio_poucas_grandes_b.csv",
"vazio_poucas_grandes_c.csv"
};

std::vector<std::string> empty_many = {
"vazio_muitas_pequenas_a.csv",
"vazio_muitas_pequenas_b.csv",
"vazio_muitas_pequenas_c.csv",
"vazio_muitas_grandes_a.csv",
"vazio_muitas_grandes_b.csv",
"vazio_muitas_grandes_c.csv"
};

std::vector<std::string> empty_few = {
"vazio_poucas_pequenas_a.csv",
"vazio_poucas_pequenas_b.csv",
"vazio_poucas_pequenas_c.csv",
"vazio_poucas_grandes_a.csv",
"vazio_poucas_grandes_b.csv",
"vazio_poucas_grandes_c.csv"
};

std::vector<std::string> full_many = {
"cheio_muitas_pequenas_a.csv",
"cheio_muitas_pequenas_b.csv",
"cheio_muitas_pequenas_c.csv",
"cheio_muitas_grandes_a.csv",
"cheio_muitas_grandes_b.csv",
"cheio_muitas_grandes_c.csv"
};

std::vector<std::string> full_few = {
"cheio_poucas_pequenas_a.csv",
"cheio_poucas_pequenas_b.csv",
"cheio_poucas_pequenas_c.csv",
"cheio_poucas_grandes_a.csv",
"cheio_poucas_grandes_b.csv",
"cheio_poucas_grandes_c.csv"
};


std::vector<std::string> test = {
"test.csv"
};



void plot_files(std::vector<std::string> files, std::string image_name, bool no_lock_unordered, std::string folder){
    std::unordered_map<std::string, std::vector<std::vector<long>>> map;
    std::unordered_map<std::string, std::vector<double>> avg_map;

    long highest = 0;
    long lowest = std::numeric_limits<long>::max();
    for (auto &&file_name : files)
    {
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
            long elapsed_time = std::stol(temp);


            if(map.find(name) == map.end()){
                std::vector<std::vector<long>> v(4);
                map.emplace(name, v);

                std::vector<double> avgv(4,0.0);
                avg_map.emplace(name, avgv);
            }

            map[name][((int)log2(threads))-2].push_back(elapsed_time);
        }
        for (const auto& kv : map) {
            int i = 0;
            for (auto list : kv.second){
                if((kv.first == "LockUnordered" || kv.first == "WFCUnorderedMap 16") && no_lock_unordered){
                    continue;
                }
                double avg;

                avg = average(list);
                avg_map[kv.first][i] = avg;
                i++;

                if(avg > highest){
                    highest = avg;
                }
                if(avg <= lowest ){
                    lowest = avg;
                }
                
                std::cout << kv.first << " " << pow(2, i+1) << " " << avg << "\n";
            }
            
        }
    }
    
    
    std::vector<double> x = {1,2,3,4};

    std::vector<std::string> legend_vector;
    if(no_lock_unordered){
        avg_map.erase("LockUnordered");
        avg_map.erase("WFCUnorderedMap 16");
    }
    for (const auto& kv : avg_map) {
        matplot::plot(x, kv.second, "-o");
        matplot::hold(matplot::on);
        legend_vector.push_back(kv.first);
    }

    matplot::xticks({0,1,2,3,4,5});
    matplot::xticklabels({" ", "4", "8","16","32", " "});
    matplot::xrange({0.8,4.2});
    std::cout << lowest << std::endl;
    std::string lowstr = std::to_string(lowest);
    for (size_t i = 1; i < lowstr.size(); i++)
    {
        lowstr[i] = '0';
    }
    std::cout << std::stol(lowstr) << std::endl;
    matplot::ylim({std::stol(lowstr)*1.0,highest*1.05});
    
    matplot::grid(matplot::on);
    matplot::xlabel("Threads");
    matplot::ylabel("Tempo de execução (ns)");
    auto lgd = matplot::legend(legend_vector);
    lgd->inside(false);
    lgd->location(matplot::legend::general_alignment::right);
    std::string path = image_name;
    path = "./"+folder+path+".png";
    matplot::save(path);
    matplot::hold(false);
}

void plot_file(std::string file, bool no_lock_unordered, std::string folder){
    std::vector<std::string> v = {file};
    plot_files(v, file, no_lock_unordered,folder);
}

void plot_all(bool no_lock_unordered, std::string folder){

    plot_files(all, "geral", no_lock_unordered, folder+"geral/");
    plot_files(small, "pequenos", no_lock_unordered, folder+"geral/");
    plot_files(big, "grandes", no_lock_unordered, folder+"geral/");

    plot_files(dist_a, "dist_a", no_lock_unordered, folder+"geral/");
    plot_files(dist_a_many, "dist_a_muitas", no_lock_unordered, folder+"geral/");
    plot_files(dist_a_few, "dist_a_poucas", no_lock_unordered, folder+"geral/");

    plot_files(dist_b, "dist_b", no_lock_unordered, folder+"geral/");
    plot_files(dist_b_many, "dist_b_muitas", no_lock_unordered, folder+"geral/");
    plot_files(dist_b_few, "dist_b_poucas", no_lock_unordered, folder+"geral/");

    plot_files(dist_c, "dist_c", no_lock_unordered, folder+"geral/");
    plot_files(dist_c_many, "dist_c_muitas", no_lock_unordered, folder+"geral/");
    plot_files(dist_c_few, "dist_c_poucas", no_lock_unordered, folder+"geral/");

    plot_files(empty, "vazia", no_lock_unordered, folder+"geral/");
    plot_files(full, "preenchida", no_lock_unordered, folder+"geral/");

    plot_files(many, "muitas", no_lock_unordered, folder+"geral/");
    plot_files(few, "poucas", no_lock_unordered, folder+"geral/");

    plot_files(empty_few, "vazia_poucas", no_lock_unordered, folder+"geral/");
    plot_files(empty_many, "vazia_muitas", no_lock_unordered, folder+"geral/");

    plot_files(full_few, "preenchida_poucas", no_lock_unordered, folder+"geral/");
    plot_files(full_many, "preenchida_muitas", no_lock_unordered, folder+"geral/");

    plot_files(many_small, "muitas_pequenos", no_lock_unordered, folder+"geral/");
    plot_files(few_small, "poucas_pequenos", no_lock_unordered, folder+"geral/");
    plot_files(many_big, "muitas_grandes", no_lock_unordered, folder+"geral/");
    plot_files(few_big, "muitas_grandes", no_lock_unordered, folder+"geral/");
    plot_files(test, "test", no_lock_unordered, folder+"geral/");

    plot_files(dist_a_big, "dist_a_grandes", no_lock_unordered, folder+"geral/");
    plot_files(dist_a_small, "dist_a_pequenos", no_lock_unordered, folder+"geral/");
    
    plot_files(dist_b_big, "dist_b_grandes", no_lock_unordered, folder+"geral/");
    plot_files(dist_b_small, "dist_b_pequenos", no_lock_unordered, folder+"geral/");

    plot_files(dist_c_big, "dist_c_grandes", no_lock_unordered, folder+"geral/");
    plot_files(dist_c_small, "dist_c_pequenas", no_lock_unordered, folder+"geral/");

     for (auto &&file : dist_a)
     {
         plot_file(file, no_lock_unordered, folder+"dist-95-5-5/");
     }

     for (auto &&file : dist_b)
     {
         plot_file(file, no_lock_unordered, folder+"dist-45-45-10/");
     }

     for (auto &&file : dist_c)
     {
         plot_file(file, no_lock_unordered, folder+"dist-5-95-5/");
     }
}
int main(int argc, char *argv[]){
    plot_all(true, "sem-lockunordered/");
    plot_all(false, "com-lockunordered/");
    
    return 0;
}

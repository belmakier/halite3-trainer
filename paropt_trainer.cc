#include <random>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <ctime>
#include <map>
#include <vector>
#include <string>

struct Genome {
  int ID;
  std::vector<double> params;
  double fitness;

  Genome() {
    ID = -1;
    fitness = 0;
  }
  Genome(int id, int nParams) {
    ID = id;
    fitness = 0;
    for (int i=0; i<nParams; ++i) {
      params.push_back(((double)(rand()%2000) - 1000.0)/1000.0);
    }
  }
  void Save(const char *fileName) {
    std::ofstream paramFile(fileName);
    for (int i=0; i<params.size(); ++i) {
      paramFile << params[i] << "\n";
    }
    paramFile.close();
  }
  void Load(int id, const char *fileName) {
    ID = id;
    double weight;
    std::ifstream paramFile(fileName);
    while (paramFile >> weight) {
      params.push_back(weight);
    }
  }
  bool operator<(const Genome& rhs) const {return fitness > rhs.fitness;}
};

std::pair<Genome, Genome> uniform_crossover(int &curID, Genome p1, Genome p2) {
  std::default_random_engine generator;
  int nParams = p1.params.size();
  Genome c1;
  Genome c2;
  c1.ID = curID;
  curID++;
  c2.ID = curID;
  curID++;
  for (int i=0; i<nParams; ++i) {
    if (rand()%2) {
      c1.params.push_back(p1.params[i]);
      c2.params.push_back(p2.params[i]);
    }
    else {
      c1.params.push_back(p2.params[i]);
      c2.params.push_back(p1.params[i]);
    }
    if (rand()%100<3){
      std::normal_distribution<double> ndist(0, 0.1*c1.params[i]);
      c1.params[i]+=ndist(generator);
    }
    if (rand()%100<3){
      std::normal_distribution<double> ndist(0, 0.1*c2.params[i]);
      c2.params[i]+=ndist(generator);
    }
    if (c1.params[i]>1.0) { c1.params[i]=1.0; }
    if (c1.params[i]<-1.0) { c1.params[i]=-1.0; }
    if (c2.params[i]>1.0) { c2.params[i]=1.0; }
    if (c2.params[i]<-1.0) { c2.params[i]=-1.0; }
  }
  
  return std::pair<Genome, Genome>(c1, c2);
}

int main(int argc, const char** argv) {

  int last_best_ID = -1;
  int mapsize = 8;
  int nPlayers = 2;
  int nSquares = mapsize*mapsize;
  int genstart = 0;

  if (argc > 1) {
    genstart = atoi(argv[1]);
  }

  srand(time(NULL));
  
  std::vector<std::string> registered_bots = {"bot_v4 bot_v4/bot_v4",
                                              "bot_v5 bot_v5/bot_v5 bot_v5/params.txt",
                                              "bot_v6 bot_v6/bot_v6 bot_v6/params_3.txt"};

  std::map<int, Genome> population;

  int popsize = 30;
  int curID = 0;    
  if (genstart == 0) {
    //random population
    for (int i=0; i<popsize; ++i) {
      population[curID] = Genome(curID, 16);
      curID++;
    }
  }
  else {
    //load from file
    std::cout << "Loading population" << std::endl;
    std::ifstream genome_list_file("gen_"+std::to_string(genstart-1)+"/genome_list.txt");
    int gNum;
    while(genome_list_file >> gNum) {
      if (curID < gNum) { curID = gNum; }
      Genome g;
      g.Load(gNum, ("gen_"+std::to_string(genstart-1)+"/bots/bot_v6.2/weights/genome_"+std::to_string(gNum)).c_str());
      population[gNum] = g;
    }
    genome_list_file.close();
  }
  
  for (int gen=genstart; gen<1000; ++gen) {
    //make new db for evaluation
    std::string folder_name("gen_"+std::to_string(gen));
    system(("mkdir "+folder_name).c_str());
    system(("cp -R mnt/* "+folder_name).c_str());
    for (auto &bot : registered_bots ) {
      system(("./register_bot.sh "+folder_name+" "+bot).c_str());
    }

    //make new folder for weights
    system(("mkdir "+folder_name+"/bots/bot_v6.2/weights").c_str());
    
    //save every genome to a file
    std::ofstream genome_list_file(folder_name+"/genome_list.txt");
    for (auto &genome : population) {
      int ID = genome.first;
      genome.second.Save((folder_name+"/bots/bot_v6.2/weights/genome_"+std::to_string(ID)).c_str());
      system(("./register_bot.sh "+folder_name+" genome_"+std::to_string(ID)+" bot_v6.2/bot_v6.2 bot_v6.2/weights/genome_"+std::to_string(ID)).c_str());
      genome_list_file << ID << std::endl;
    }

    genome_list_file.close();
    
    int nIters = 150;
    int nCores = 3;
    //play lots of games
    system(("./evaluate.sh "+folder_name+" -i "+std::to_string(nIters)+" -j "+std::to_string(nCores)).c_str());

    //print nice results
    system(("./pretty_print.sh "+folder_name).c_str());
    
    //set fitness = mu - 3*sigma
    system(("./get_fitnesses.sh "+folder_name).c_str());

    std::ifstream fitnessfile("fitnesses.txt");
    double fitness;
    int ID;
    std::vector<Genome> genomes;
    while (fitnessfile >> fitness >> ID) {
      population[ID].fitness = fitness;
      genomes.push_back(population[ID]);
    }

    std::sort(genomes.begin(), genomes.end());
    int best_ID = genomes[0].ID;

    for (int i=0; i<genomes.size(); ++i) {
      std::cout << genomes[i].ID << "   " << genomes[i].fitness << std::endl;
    }
    
    population[best_ID].Save(("mnt/bots/bot_v6.2/best_genomes/genome_"+std::to_string(best_ID)+".txt").c_str());

    //select best 10 genomes and do crossover
    std::vector<Genome> parents;
    for (int i=0; i<5; ++i) {
      parents.push_back(genomes[i]);
    }
    parents.push_back(genomes[10]);
    parents.push_back(genomes[11]);
    parents.push_back(genomes[12]);
    
    parents.push_back(genomes[20]);
    parents.push_back(genomes[21]);

    std::cout << "Parents are: "<< std::endl;
    for (int i=0; i<parents.size(); ++i) {
      std::cout << parents[i].ID << std::endl;
    }

    population.clear();

    for (int i=0; i<parents.size(); ++i) {
      population[parents[i].ID] = parents[i];
    }

    std::cout << "Breeding" << std::endl;
    for (int i=0; i<(popsize-parents.size())/2; ++i) {
      auto children = uniform_crossover(curID, parents[rand()%10], parents[rand()%10]);
      population[children.first.ID] = children.first;
      population[children.second.ID] = children.second;
    }                            

    std::cout << "Gen " << gen << " finished, new pop size " << population.size() << std::endl;
    // if (best_ID > last_best_ID) {
    //   registered_bots.push_back("bot_v6.2_gen"+std::to_string(gen)+" bot_v6.2/bot_v6.2 bot_v6.2/best_genomes/genome_"+std::to_string(best_ID)+".txt");
    //   last_best_ID = best_ID;
    // }
  }
            
}

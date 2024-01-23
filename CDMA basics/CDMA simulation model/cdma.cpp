#include <iostream>
#include <fstream>
#include <vector>

class CDMA {
public:
    class Client {
    public:
        Client(): id(0) {}
        Client(size_t i, std::vector<int>& code, std::vector<int>& data): code(code), c_data(data), id(i + 1) {}

        std::vector<int> getCode(){
            return code;
        }
        std::vector<int> getData(){
            return c_data;
        }
        void setData(std::vector<int>& d){
            c_data = d;
        }
        void setCode(std::vector<int>& c){
            code = c;
        }
        int getId() const{
            return id;
        }

        void viewClient() const {
            std::cout<<"\n----------------------------------------------------------";
            std::cout<< "\n Client: "<<getId()<<std::endl;
            std::cout << "\n Code: ";
            for(auto x: code)std::cout << x;
            std::cout<<std::endl;
            std::cout << "\n Data: ";
            for(auto x: c_data)std::cout << x;
            std::cout<<std::endl;
            std::cout<<"----------------------------------------------------------\n";
        }
    private:
        std::vector<int> code;
        std::vector<int> c_data;
        int id;
    };

    void load() {
        std::ifstream fileCodes("Codes.txt");
        if (!fileCodes.is_open()) {
            std::cerr << "Could not open Codes.txt!" << std::endl;
            return;
        }

        std::string kod;
        while (std::getline(fileCodes, kod)) {
            codes.push_back(string2vec(kod));
        }

        fileCodes.close();

        std::ifstream fileData("Data.txt");
        if (!fileData.is_open()) {
            std::cerr << "Could not open Data.txt!" << std::endl;
            return;
        }

        std::string dana;
        while (std::getline(fileData, dana)) {
            data.push_back(string2vec(dana));
        }

        fileData.close();
    }

    std::vector<std::vector<int>> getCodes(){
        return codes;
    }

    std::vector<int> string2vec(std::string str){
        std::vector<int> values;
        for (int i = 0; i < str.size(); ++i) {
            if (str[i] == '1') {
                values.push_back(1);
            } else values.push_back(-1);
        }
        return values;
    }

    int getSizeClients(){
        return clients.size();
    }
    void view() const {
        for(const auto& x : clients){
            x.viewClient();
        }
    }
    void createClients() {
        for (size_t i = 0; i < codes.size(); ++i) {
            CDMA::Client newClient(i, codes[i], data[i]);
            clients.push_back(newClient);
        }
    }
    CDMA::Client getClient(int ind){
        return clients[ind];
    }

    std::vector<std::vector<int>> codeClientsData(){
        std::vector<std::vector<int>> coded_vec;
        for(int i = 0; i<getSizeClients();i++){
            coded_vec.push_back(codeData(getClient(i)));
        }
        return coded_vec;
    }

    std::vector<std::vector<int>> decodeClientsData(std::vector<int> sig){
        std::vector<std::vector<int>> decoded_vec;
        for(int i = 0; i<getSizeClients();i++){
            decoded_vec.push_back(decodedSignal(sig, getClient(i).getCode()));
        }
        return decoded_vec;
    }

    void viewCodedClients(std::vector<std::vector<int>> coded_vec){
        std::cout<<"\n----------------------------------------------------------";
        for(int i = 0; i<coded_vec.size();i++){

            std::cout<<"\nCoded"<<i+1<<": ";
            for(int j = 0; j<coded_vec[i].size();j++){
                std::cout<<coded_vec[i][j];
            }

            std::cout<<std::endl;
        }
        std::cout<<"\n---------------------------------------------------------";

        std::cout<<std::endl;
    }

    void viewDecodedClients(std::vector<std::vector<int>> decoded_vec){
        for (int i = 0; i<decoded_vec.size();i++){
            std::cout<<"\nDecoded data "<<i<<": ";
            for(int j =0; j<decoded_vec[i].size();j++){
                std::cout<<decoded_vec[i][j];
            }
            std::cout<<std::endl;
        }
    }

    void viewSignal(std::vector<int> sig){
        std::cout<<"\nCoded signal: ";
        for (const auto& x : sig){
            std::cout<<x;
        }
        std::cout<<std::endl;
    }

    std::vector<int> kronecker(const std::vector<int>& A, const std::vector<int>& B) {
        std::vector<int> result;
        for (double a : A) {
            for (double b : B) {
                result.push_back(a * b);
            }
        }
        return result;
    }

    std::vector<int> codeData(CDMA::Client client){
        auto coded = kronecker(client.getData(), client.getCode());
        return coded;
    }

    std::vector<int> genSignal(const std::vector<std::vector<int>>& vectors) {
        if (vectors.empty()) {
            return {};
        }

        size_t size = vectors[0].size();
        std::vector<int> result(size, 0);

        for (const auto& vec : vectors) {
            for (size_t i = 0; i < size; ++i) {
                result[i] += vec[i];
            }
        }
        return result;
    }

    std::vector<int> decodedSignal(const std::vector<int>& sig, const std::vector<int>& kod) {
        std::vector<int> decoded;
        for (size_t i = 0; i < sig.size(); i += kod.size()) {
            int sum = 0;
            for (size_t j = 0; j < kod.size(); ++j) {
                sum += sig[i + j] * kod[j];
            }
            decoded.push_back((sum > 0) ? 1 : -1);
        }
        return decoded;
    }
    
private:
    std::vector<std::vector<int>> codes;
    std::vector<std::vector<int>> data;
    std::vector<CDMA::Client> clients;
};

void CDMA_simulation() {
    CDMA base;
    base.load();
    base.createClients();
    base.view();

    std::vector<std::vector<int>> coded_vec = base.codeClientsData();
    base.viewCodedClients(coded_vec);

    auto sig = base.genSignal(coded_vec);
    base.viewSignal(sig);

    std::vector<std::vector<int>> decoded_vec = base.decodeClientsData(sig);
    base.viewDecodedClients(decoded_vec);
    
}

int main(){
    CDMA_simulation();
    system("pause");
    return 0;
}

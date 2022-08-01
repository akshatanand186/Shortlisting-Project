#include <bits/stdc++.h>
#include <string>
using namespace std;
#define ll long long
#define nl << '\n'
#define sp << " " <<

#define debug(n) cout << #n << " : " << n << '\n';
#define debug2(n1,n2) cout << #n1 << " : " << n1 << "  " << #n2 << " : " << n2 << '\n';

const int gatecutoffgenmtech = 450, gatecutoffmtechobc = 405, gatecutoffmtechscstpwd = 302;
const int gatecutoffgenbtech = 500, gatecutoffbtechobc = 450, gatecutoffbtechscstpwd = 335;

struct details{
    string name;
    string scholar;
    string category;
    double class10percentage;
    double class12percentage;
    double class10cg;
    double class12cg;
    string btechclg;
    string mtechclg;
    double btechcg;
    double btechpercent;
    double mtechcg;
    double mtechpercent;
    int gatescore;
    bool net_ugc;
};


vector<string> iits = {"Bombay","Delhi", "Kharagpur", "Guwahati", "Bhubaneswar","Madras","Roorkee","Ropar","Gandhinagar","Hyderabad","Jodhpur","Patna","Indore","Mandi","Varanasi","Palakkad","Tirupati","Dhanbad","Bhilai","Dharwad","Jammu","Goa","BHU"};

//This bool function returns whether the candidate is a passout from an iit
bool check_iit(string s){
    for(int i=0;i<iits.size();i++){
        string p = "IIT " + iits[i];
        if(s==p) return true;
    }
    for(int i=0;i<iits.size();i++){
        string p = "Indian Institute of Technology " + iits[i];
        if(s==p) return true;
    }
    return false;
}

//creates a csv file
void write_csv(string filename,vector<vector<string>>details);

pair<bool,string> isstudenteligible(struct details student){
    // Only if a student is a Insitute scholar (MHRD Funding) or Institute Scholar UGC-CSIR Funding 
    if((student.scholar != "INSTITUTE SCHOLAR(MHRD FUNDING)" && student.scholar != "INSTITUTE SCHOLAR(UGC-CSIR FUNDING)")){
        return {false, "Result not published"};
    }
    // Section 1.3
    // Checks whether the candidate is selected through net_ugc
    // If yes it directly shorlists the candidate
    if(student.net_ugc){
        return {true,"Selected through UGC-NET"};
    }

    // Section 1.2
    // Checks whether candidate is an iitian and checks for mtech cgpa
    // If metch cgpa is graeter than 8.5 then candidate is directly shortlisted
    if(check_iit(student.mtechclg) && student.mtechcg >= 8.5){
        return {true,"Selected through MTech in IIT with >= 8.5CG"};
    }

    // Section 1.1
    // Checks whether candidate is an iitian and if highest qualification is btech
    // Then if btech cgpa is greater than 8 then candidate is directly shortlisted
    if(student.mtechclg=="NA" && student.btechcg >= 8.0 && check_iit(student.btechclg)){
        return {true,"Selected through Btech in IIT with >= 8.0CG"};
    }
    // Section 3
    // if highest qualification of the candidate is mtech
    // Then we check for mtech column not to be empty
    if(student.mtechclg!="NA"){

        // We check for category of the candidate
        // Then check for the gate scores for qualification
        if(((student.category=="GEN" || student.category=="EWS") && student.gatescore<gatecutoffgenmtech) || (student.category=="OBC" && student.gatescore<gatecutoffmtechobc) || ((student.category=="SC" || student.category=="ST" || student.category=="PWD") && student.gatescore<gatecutoffmtechscstpwd)){
            // Returns false i.e the candidate is not qualified due to gate score
            return {false,"Gate Score not qualified"};
        }

        // cutoff cgpa variable is set for btech and mtech
        double cutoff_cgmtech = 6.5;
        double cutoff_cgbtech = 6.5;
        double cutoff_btechpercent = 60;
        double cutoff_mtechpercent = 60;

        // cutoff variable is reduced by 0.5 if candidate belongs to sc or st or pwd category
        if(student.category=="SC" || student.category=="ST" || student.category=="PWD"){
            cutoff_cgmtech = cutoff_cgmtech - 0.5;
            cutoff_cgbtech = cutoff_cgbtech - 0.5;
            cutoff_btechpercent = cutoff_btechpercent - 5;
            cutoff_mtechpercent = cutoff_mtechpercent - 5;
        }

        // if candidate btech or mtech cgpa is not greater than cutoff cgpa then he is not qualified
        if(student.btechcg != -1 && student.btechcg<cutoff_cgbtech){
            // Returns false i.e the candidate is not qualified due to btech cgpa not qualified
            return {false,"Btech CG not qualified"};
        }
        if(student.btechpercent != -1 && student.btechpercent<cutoff_btechpercent){
            // Returns false i.e the candidate is not qualified due to btech percent not qualified
            return {false,"Btech Percent not qualified"};
        }

        if(student.mtechcg != -1 && student.mtechcg<cutoff_cgmtech){
            // Returns false i.e the candidate is not qualified due to mtech cgpa not qualified
            return {false,"Mtech CG not qualified"};
        }
        if(student.mtechpercent != -1 && student.mtechpercent<cutoff_mtechpercent){
            // Returns false i.e the candidate is not qualified due to mtech percent not qualified
            return {false,"Mtech Percent not qualified"};
        }

        // cutoff percentage variable is set for 10th and 12th classes
        double cutoff_10th = 60;
        double cutoff_12th = 60;
        double cutoff_10thcg = 6.5;
        double cutoff_12thcg = 6.5;

        // cutoff variable is reduced by 5 percent if candidate belongs to sc or st or pwd category
        if(student.category=="SC" || student.category=="ST" || student.category=="PWD"){
            cutoff_10th -= 5;
            cutoff_12th -= 5;
            cutoff_10thcg -= 0.5;
            cutoff_12thcg -= 0.5;
        }
        double decidingcutoff10;
        double decidingcutoff12;
        double class10marks;
        double class12marks;
        if(student.class10percentage!=-1){
            class10marks = student.class10percentage;
            decidingcutoff10 = cutoff_10th;
        }
        else{
            class10marks = student.class10cg;
            decidingcutoff10 = cutoff_10thcg;
        }
        if(student.class12percentage!=-1){
            class12marks = student.class12percentage;
            decidingcutoff12 = cutoff_12th;
        }
        else{
            class12marks = student.class12cg;
            decidingcutoff12 = cutoff_12thcg;
        }
        // if candidate's 10th or 12th class percentage is not greater than cutoff percentage then he is not qualified
        if(!((class10marks>=decidingcutoff10 && class12marks>=(0.9*decidingcutoff12)) || (class10marks>=(0.9*decidingcutoff10) && class12marks>=decidingcutoff12))){
            // Returns false i.e the candidate is not qualified due to 10th or 12th class cgpa not qualified
            return {false,"Class 10-12 Percentage/CG not qualified"};
        }

        // As the candidate qualified all the above conditions he is qualified
        return {true,"Qualified through Gate And MTech"};
    }

    /*Btech through Gate*/

    // Section 2
    // if highest qualification of the candidate is btech
    // We check for category of the candidate
    // Then check for the gate scores for qualification
    if(((student.category=="GEN" || student.category=="EWS") && student.gatescore<gatecutoffgenbtech) || (student.category=="OBC" && student.gatescore<gatecutoffbtechobc) || ((student.category=="SC" || student.category=="ST" || student.category=="PWD") && student.gatescore<gatecutoffbtechscstpwd)){
            // Returns false i.e the candidate is not qualified due to gate score not qualified
        return {false,"Gate Score not qualified"};
    }

    // cutoff cgpa variable is set for btech
    double cutoff_cgbtech = 7.5;
    double cutoff_btechpercent = 70;

    // cutoff variable is reduced by 0.5 if candidate belongs to sc or st or pwd category
    if(student.category=="SC" || student.category=="ST" || student.category=="PWD"){
        cutoff_cgbtech = cutoff_cgbtech - 0.5;
    }

    // if candidate's btech cgpa is not greater than cutoff cgpa then he is not qualified
    if(student.btechcg != -1 && student.btechcg<cutoff_cgbtech){
        // Returns false i.e the candidate is not qualified due to btech cgpa not qualified
        return {false,"Btech CG not qualified"};
    }
    if(student.btechpercent != -1 && student.btechpercent<cutoff_btechpercent){
        // Returns false i.e the candidate is not qualified due to btech percent not qualified
        return {false,"Btech Percent not qualified"};
    }

    // cutoff percentage variable is set for 10th and 12th classes
    double cutoff_10th = 70;
    double cutoff_12th = 70;
    double cutoff_10thcg = 7.0;
    double cutoff_12thcg = 7.0;
    // cutoff variable is reduced by 5 percent if candidate belongs to sc or st or pwd category
    if(student.category=="SC" || student.category=="ST" || student.category=="PWD"){
        cutoff_10th -= 5;
        cutoff_12th -= 5;
        cutoff_10thcg -= 0.5;
        cutoff_12thcg -= 0.5;
    }
    double decidingcutoff10;
    double decidingcutoff12;
    double class10marks;
    double class12marks;
    if(student.class10percentage!=-1){
        class10marks = student.class10percentage;
        decidingcutoff10 = cutoff_10th;
    }
    else{
        class10marks = student.class10cg;
        decidingcutoff10 = cutoff_10thcg;
    }
    if(student.class12percentage!=-1){
        class12marks = student.class12percentage;
        decidingcutoff12 = cutoff_12th;
    }
    else{
        class12marks = student.class12cg;
        decidingcutoff12 = cutoff_12thcg;
    }
    // if candidate's 10th or 12th class percentage is not greater than cutoff percentage then he is not qualified
    if(!((class10marks>=decidingcutoff10 && class12marks>=(0.9*decidingcutoff12)) || (class10marks>=(0.9*decidingcutoff10) && class12marks>=decidingcutoff12))){
        // Returns false i.e the candidate is not qualified due to 10th or 12th class cgpa not qualified
        return {false,"Class 10-12 Percentage/CG not qualified"};
    }

    // As the candidate qualified all the above conditions he is qualified
    return {true,"Qualified through Gate And BTech"};

}

vector<pair<string,vector<string>>> read_csv(string filename);

int main(){
    vector<pair<string,vector<string>>> v1 = read_csv("final.csv");
    vector<vector<string>> answer(v1[0].second.size());
    for(int i = 0; i < v1[0].second.size(); i++){
        struct details s1;
        // Obtaining data from the excel sheet from different columns
        s1.name = (v1[0].second)[i];
        s1.scholar = (v1[1].second)[i];
        s1.category = (v1[5].second)[i];
        if((v1[6].second)[i]=="YES") s1.category = "PWD";
        s1.class10percentage = (v1[11].second[i]=="")?-1:stod((v1[11].second)[i]);
        s1.class12percentage = (v1[18].second[i]=="")?-1:stod((v1[18].second)[i]);
        s1.class10cg = (v1[12].second[i]=="")?-1:stod((v1[12].second)[i]);
        s1.class12cg = (v1[19].second[i]=="")?-1:stod((v1[19].second)[i]);
        s1.btechclg = v1[22].second[i];
        s1.mtechclg = v1[30].second[i];
        s1.btechcg = (v1[27].second[i]=="" ||v1[27].second[i]=="NA" )?-1:stod((v1[27].second)[i]);
        s1.mtechcg = (v1[35].second[i]=="" ||v1[35].second[i]=="NA" )?-1:stod((v1[35].second)[i]);
        s1.btechpercent = (v1[26].second[i]=="" ||v1[26].second[i]=="NA" )?-1:stod((v1[26].second)[i]);
        s1.mtechpercent = (v1[34].second[i]=="" ||v1[34].second[i]=="NA" )?-1:stod((v1[34].second)[i]);
        s1.gatescore = (v1[48].second[i]=="" ||v1[48].second[i]=="NA" )?-1:stod((v1[48].second)[i]);
        s1.net_ugc = (v1[53].second[i]=="" || v1[53].second[i] =="NA")? false:true;

        pair<bool,string> detailsofqualific = isstudenteligible(s1);
        answer[i].push_back(s1.name);
        answer[i].push_back(to_string(detailsofqualific.first));
        answer[i].push_back(detailsofqualific.second);
    }

    write_csv("answer.csv",answer);



    return 0;
}

void write_csv(string filename,vector<vector<string>>details){
    ofstream myFile(filename);
    myFile << "Name,Result,Reason\n";
    for(int i=0;i<details.size();i++){
        myFile << details[i][0] << "," << details[i][1] << "," << details[i][2] nl;
    }
}

vector<pair<string,vector<string>>> read_csv(string filename){
    // Reads a CSV file into a vector of <string, vector<int>> pairs where
    // each pair represents <column name, column values>

    // Create a vector of <string, int vector> pairs to store the result
    vector<pair<string,vector<string>>> result;

    // Create an input filestream
    ifstream myFile(filename);

    // Make sure the file is open
    if(!myFile.is_open()) throw std::runtime_error("Could not open file");

    // Helper vars
    string line, colname;
    string val;

    // Read the column names
    if(myFile.good()){
        // Extract the first line in the file
        getline(myFile, line);

        // Create a stringstream from line
        stringstream ss(line);

        // Extract each column name
        while(getline(ss, colname, ',')){

            // Initialize and add <colname, int vector> pairs to result
            result.push_back({colname, vector<string> {}});
        }
    }

    // Read data, line by line
    while(getline(myFile, line)){
        // Create a stringstream of the current line
        stringstream ss(line);

        // Keep track of the current column index
        int colIdx = 0;
        int flag = 0;
        // Extract each integer
        while(getline(ss, val, ',')){
            if(val=="$") {flag = 1; break; }
            // Initialize and add <colname, int vector> pairs to result
            result.at(colIdx).second.push_back(val);
            colIdx++;
        }
        if(flag) break;

    }

    myFile.close();

    return result;
}
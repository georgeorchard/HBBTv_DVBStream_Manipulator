#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <fstream>


class TransportStream{
	private:
		std::string input;
		std::string output;
	
	public:
		/**
		Constructor function
		Parameters:
		inputFile(String): The name of the input file
		*/
		TransportStream(std::string inputFile){
			
			//Create the output file name
			//get file name no .ts
			std::string substring = originalString.substr(0, input.length() - 3);
			// Get the current system time
			auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

			// Convert the current time to a local time structure
			struct std::tm* timeInfo = std::localtime(&now);

			// Format the time as YYYYMMDDHHMMSS
			std::stringstream ss;
			ss << std::put_time(timeInfo, "%Y%m%d%H%M%S");

			// Get the formatted time as a string
			std::string formattedTime = ss.str();
			//create the output string
			std::string outputFile = substring + "_Processed_" + formattedTime + ".ts";
			this->output = ouputFile;
		}
		
		/*
		Getter for input
		Parameters:
		Null
		Returns:
		input(String): The input file name
		*/
		std::string getInput(){
			return input;
		}
		
		/*
		Getter for output
		Parameters:
		Null
		Returns:
		output(String): The output file name
		*/
		std::string getOutput(){
			return output;
		}
		
		/*
		Function for processing multiple (or single) request on the file
		*/
		void processMultiple(){
			//process one file here using intermediate.ts and output
			
			
			//choice to do more
			std::cout << "\nAnother Channel?:" << std::endl;
			std::cout << "0. No" << std::endl;
			std::cout << "1. Yes" << std::endl;
			int choice;
			std::cout << "Enter choice (0 or 1): ";
			std::cin >> choice;
			if(choice == 1){
				//copy the output file to the intermediate file and go again
				std::string sourceFileName = output; 
				std::string destinationFileName = "intermediate.ts"; 
				std::ifstream sourceFile(sourceFileName, std::ios::binary);
				std::ofstream destinationFile(destinationFileName, std::ios::binary | std::ios::out | std::ios::trunc);
				destinationFile << sourceFile.rdbuf();
				sourceFile.close();
				destinationFile.close();
				//call function again
				processMultiple();
			}
		}
		
	};	
	
		
		
		
		
	int main(int argc, char* argv[]){
		//get the file name from the command line
		std::string input = argv[0];
		//Add .ts to end of file name if not there
		if(!(endsWith(input, ".ts"))){
			input = input + ".ts";
		}
		
		//copy the input file to an intermediate one
		std::string sourceFileName = input; 
		std::string destinationFileName = "intermediate.ts"; 
		std::ifstream sourceFile(sourceFileName, std::ios::binary);
		std::ofstream destinationFile(destinationFileName, std::ios::binary | std::ios::out | std::ios::trunc);
		destinationFile << sourceFile.rdbuf();
		sourceFile.close();
		destinationFile.close();
		
		//create the transport stream object
		TransportStream ts(input);
		//call the process multiple
		ts.processMultiple()
		//finish
		return 0;
	}	
		
}

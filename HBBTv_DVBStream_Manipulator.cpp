#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <cstring>
#include <cmath>
#include <iomanip>
#include <stdexcept>



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
			//set input as intermediate file
			this->input = "inputcopy.ts"
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
		
		/**
		Getter for input
		Parameters:
		Null
		Returns:
		input(String): The input file name
		*/
		std::string getInput(){
			return input;
		}
		
		/**
		Getter for output
		Parameters:
		Null
		Returns:
		output(String): The output file name
		*/
		std::string getOutput(){
			return output;
		}
		

		/**
		Function for processing multiple (or single) request on the file
		*/
		void processMultiple(){
			//process one file here using input and output
			//Get the XML for the input file
			getXML(input);

			//save the PAT of the XML of the input file in a seperate file
			//save_pat()

			//Get the process number and pmtPID of the chosen service
			//choices = serviceChoice()
			//processNumber = choices[0]
			//pmtPID = choices[1]

			//process the file
			processTSFile(processNumber, pmtPID);
			
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

		/**
		* Function for processing one channel
		* Parameters:
		* processNumber(int): The process number of the channel
		* pmtPID(int): The PMT pid of the process
		* Returns:
		* None
		*/
		void processTSFile(int processNumber, int pmtPID) {
			//Make the PMT XML for the process number 
			//savePMTByService("pmtXML.xml", processNumber)
			
			//Replace SCTE-35 with DSMCC
			
			//Get SCTE PID
			//Check if SCTE PID not null
			//Replace null choice
			std::cout << "\nReplace Null SCTE Packets?:" << std::endl;
			std::cout << "0. No" << std::endl;
			std::cout << "1. Yes" << std::endl;
			int choice;
			std::cout << "Enter choice (0 or 1): ";
			std::cin >> choice;
			//if replaceing null
			if (choice == 0) {
				replace_scte35(input, "intermediate.ts", int_scte_pid, False)
			}
			//if not replacing null
			else {
				replace_scte35(input, "intermediate.ts", int_scte_pid, True)
			}
			//Replace SCTE elements with DSMCC ones
			//replaceSCTEElement("pmtXML.xml", scte_pid)
			//create new pmt
			//replaceSCTEElement("pmtXML.xml", scte_pid)
			//replace old pmt with new one
			//replace_table("intermediate.ts", pmt_pid, "pmtXML.xml", output_file)
				
			//add that DSMCC element to another service
			bool replaceChoice = true;
			while (replaceChoice) {
				std::cout << "\nAdd DSMCC Element to another service?:" << std::endl;
				std::cout << "0. No" << std::endl;
				std::cout << "1. Yes" << std::endl;
				int choice;
				std::cout << "Enter choice (0 or 1): ";
				std::cin >> choice;
				if (choice == 0) {
					replaceChoice = false;
				}
				else {
					//addDSMCCToService("pmtXML.xml", scte_pid, output)
				}

			}

			

		}

		//TABLE MANIPULATION FUNCTIONS
		
		/**
		* Function to insert a table (AIT) to the TS at a given insertRate
		* Parameters:
		* input_file(String): The input file
		* pid(int): The PID to have the table on
		* tablexml (str): The table XML to inject.
		* reprate_ms (int): The repetition rate in milliseconds.
		* output_file (str): The output file.
		*Returns:
		*None
		*/
		void insert_table(std::string input_file ,int pid, std::string tablexml, int reprate_ms, std::string output_file) {
			std::string command = 'tsp -I file ' + input_file + ' -P inject -p ' + (std::to_string(pid)) + tablexml + '=' + (std::to_string(reprate_ms)) + ' -O file ' + output_file;
			system(command);
		}

		/**
		* Function to replace a table 
		* Parameters:
		* pid(int): The PID to have the table on
		* tablexml (str): The table XML to inject.
		* reprate_ms (int): The repetition rate in milliseconds.
		* output_file (str): The output file.
		*Returns:
		*None
		*/
		void replace_table(std::string input_file, int pid, std::string tablexml, int reprate_ms, std::string output_file) {
			std::string command = 'tsp -I file ' + input_file + ' -P inject -p ' + (std::to_string(pid)) + tablexml + '=' + (std::to_string(reprate_ms)) + ' -O file ' + output_file;
			system(command);
		}


		//SCTE REPLACEMENT FUNCTIONS
		/**
		* Function to calculate the CRC of a section
		* Parameters:
		* section(string): The string of hex bytes that is the section
		* Returns:
		* crc(int): 32 bit integer that is CRC.
		*/
		unsigned int calculate_section_crc(std::string& section) {
			// Convert section from hex string to bytes (assuming the input is already in byte format)
			std::string section_bytes = section; 

			// Initialize the CRC value
			unsigned int crc = 0xFFFFFFFF;

			// CRC-32 polynomial
			unsigned int polynomial = 0x04C11DB7;

			// Calculate the CRC
			for (char byte : section_bytes) {
				crc ^= static_cast<unsigned int>(byte) << 24;
				for (int i = 0; i < 8; ++i) {
					if (crc & 0x80000000) {
						crc = (crc << 1) ^ polynomial;
					}
					else {
						crc <<= 1;
					}
				}
			}

			// Convert the CRC value to hex string
			std::stringstream ss;
			ss << std::hex << (crc & 0xFFFFFFFF);
			std::string crc_hex = ss.str();
			crc_hex = std::string(8 - crc_hex.length(), '0') + crc_hex; // Zero padding if necessary
			std::transform(crc_hex.begin(), crc_hex.end(), crc_hex.begin(), ::toupper);


			return (crc & 0xFFFFFFFF);
		}


		/**
		* Function to send a stuffed packet on the output_stream
		* Parameters:
		* output_stream(file): The output stream to send the stuffed packet on
		* Returns:
		* Null
		*/
		void sendStuffedPacket(std::ofstream& output_stream) {
			// Creating a stuffed packet
			uint8_t stuffed_packet[] = { 0x47 };
			const char additional_bytes[] = "\x1F\xFF\x10";
			const uint8_t repeating_byte = 0xFF;

			// Appending bytes to the packet
			output_stream.write(reinterpret_cast<char*>(stuffed_packet), sizeof(stuffed_packet));
			output_stream.write(additional_bytes, sizeof(additional_bytes) - 1); // -1 to exclude null terminator

			// Appending repeated bytes to complete the packet
			for (int i = 0; i < 184; ++i) {
				output_stream.write(reinterpret_cast<const char*>(&repeating_byte), sizeof(repeating_byte));
			}
		}

		/**
		* Function to build a DSMCC packet from a SCTE payload
		* 
		* Parameters:
		* scte35_payload (packet[]): The payload packets of the SCTE35
		* version_count (int): The version of the DSMCC payload
		* packet (packet): The SCTE35 packet.
		* cont_count (int): The continuity counter.
		* 
		* Returns:
		* DSMCC Packet
		*/
		std::vector<uint8_t> buildDSMCCPacket(const std::vector<uint8_t>& scte35_payload, int version_count, const std::vector<uint8_t>& packet, int cont_count) {
			//DESCRIPTOR LIST SECTION - SPLICE INFORMATION - [A178-1r1_Dynamic-substitution-of-content  Table 3] - This information just goes before the SCTE35 data
			//24 bits
			//8 bits: DVB_data_length
			//3 bits: reserved for future use
			//1 bit: event type
			//4 bits: timeline type
			//8 bits: private data length
			std::vector<uint8_t> dsm_descriptor = { 0x01, 0xE1, 0x00 }; // DSM Descriptor Initialization

			// Adding SCTE35 payload to DSM Descriptor
			dsm_descriptor.insert(dsm_descriptor.end(), scte35_payload.begin(), scte35_payload.end());

			// Base64 encode the SCTE35 payload
			std::vector<uint8_t> encoded_payload = base64_encode(dsm_descriptor);



			//DATA IN BEFORE THE DSMCC SECTION FORMAT - STREAM DATA
			//8 bits
			std::vector<uint8_t> dsmcc_packet = { 0x47 };

			// Add next 16 bits from the packet
			dsmcc_packet.insert(dsmcc_packet.end(), packet.begin() + 1, packet.begin() + 3);

			//8 bits
			uint8_t byte4 = cont_count | 0x10;
			dsmcc_packet.push_back(byte4);



			// DSMCC PACKET SECTION - [ISO/IEC 13818-6:1998  Table 9-2]
			//Length of DSM - CC Packet
			//4 is the data that goes in before the table_id(stream data)

			//6 (should be 5) as this is the data after the dsmcc_section_length field and before we put the dsmcc descriptor field in
			//encoded payload is the splice information from SCTE35
			//4 (should be 12) as this is the length of the streamEventDescriptor without the private data bytes)

			//8 is the CRC_32
			int dsmcc_len = 6 + encoded_payload.size() + 4 + 8 + 4;

			//8 bits - Table ID
			//x3D means that section contains stream descriptors - [ISO/IEC 13818-6:1998  Table 9-3]
			dsmcc_packet += {0x00, 0x3D}; // Table ID

			//8 bits
			//1 bit: section_syntax_indicator
			//1 bit: private_indicator
			//2 bits: reserved
			//4 bits: start of DSMCC_section_length(length of everything after this field)
			uint16_t dsmcc_siglen = dsmcc_len - 1;
			dsmcc_packet.push_back(((dsmcc_siglen & 0x0F00) >> 8) + 0xB0);

			//8 bits: rest of DSMCC section length
			dsmcc_packet.push_back(dsmcc_siglen & 0x00FF);

			// TID Ext, do-it-now       ETSI TS 102 809 V1.2.1 / Section B32.  TID Ext = EventId 1 (14 bits), Bits 14/15 zero = 0x0001
			//16 bits - table_id_extension(do - it - now)
			dsmcc_packet += {0x00, 0x01}; // table_id_extension

			// Version field
			//Version 1 (RR / VVVVV / C)   RR / 5 BIts of Version number / Current / Next indicator(always 1)   Version 1 = 11000011 = C3
			//Mask version count to 5 bits so cycles round.
			version_count &= 0b11111;
			uint8_t version_field = 0xC0 + (version_count << 1) + 0x01;
			//8 bits
			//2 bits: reserved
			//5 bits: version_number
			//1 bit: current_next_indicator
			dsmcc_packet.push_back(version_field);
			
			//16 bits
			//8 bits: section
			//8 bits: last section
			dsmcc_packet += {0x00, 0x00};



			// STREAM EVENT DESCRIPTOR SECTION - [ISO/IEC 13818-6:1998  Table 8-6]
			//8 bits - descriptorTag - x1a = 26 which is Stream Event Descriptor
			dsmcc_packet.push_back(0x1A); // Descriptor Tag

			//8 bits - Descriptor length (think this should be 10 + len(encoded_payload))
			int dsmcc_payload_len = encoded_payload.size() + 4;
			dsmcc_packet.push_back(dsmcc_payload_len & 0x00FF);

			//80 bits - rest of descriptor
			//16 bits: eventID
			//31 bits: reserved
			//33 bits: eventNPT
			dsmcc_packet += {0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFE, 0x00, 0x00, 0x00, 0x00}; // Remaining descriptor

			//THE PRIVATE DATA BYTES THE SCTE SECTION -  Add the SCTE35 payload into the DSMCC Packet
			dsmcc_packet.insert(dsmcc_packet.end(), encoded_payload.begin(), encoded_payload.end());

			//32 Bits - The CRC_32 Section as sectionSyntaxIndicator == 1 FINAL PART FROM [ISO/IEC 13818-6:1998  Table 9-2]
			uint32_t dsmcc_crc = calculate_section_crc(&dsmcc_packet[5], dsmcc_len + 3 - 5);
			for (int i = 0; i < 4; ++i) {
				dsmcc_packet.push_back((dsmcc_crc >> (8 * (3 - i))) & 0xFF);
			}

			// Padding to make the packet 188 bytes
			while (dsmcc_packet.size() < 188) {
				dsmcc_packet.push_back(0xFF);
			}

			return dsmcc_packet;
		}

		/**
		A function that replaces SCTE35 packets in a Transport Stream with DSMCC ones.

		Parameters:
		input_file(String) : The name of the file containig the input.
		output_file(String) : The name of the file for the output.
		scte35_pid(int) : The PID of the SCTE packets.
		replaceNull(boolean) : The option to replace null packets

		Returns :
		null.
		*/
		void replace_scte35(const std::string& input_file, const std::string& output_file, int scte35_pid, bool replaceNull) {
			int packetcount = 0;
			int events_replaced = 0;
			int events_notreplaced = 0;
			int version_count = 1;
			int cont_count = 1;
			int adaptation_len = 0;

			std::ifstream input_stream(input_file, std::ios::binary);
			std::ofstream output_stream(output_file, std::ios::binary);

			if (!input_stream.is_open() || !output_stream.is_open()) {
				throw std::runtime_error("Error opening files");
			}

			std::cout << "\nSearching for SCTE35 Payload on PID: " << scte35_pid << std::endl;

			while (true) {
				char sync_byte;
				input_stream.read(&sync_byte, 1);

				if (!input_stream) {
					break; // End of file
				}

				if (sync_byte != '\x47') {
					throw std::runtime_error("Invalid sync byte");
				}

				std::vector<uint8_t> packet(188);
				packet[0] = sync_byte;
				input_stream.read(reinterpret_cast<char*>(&packet[1]), 187); // Read the entire packet

				// Extract packet PID
				int pid = (packet[1] << 8) | packet[2];
				pid &= 0x1FFF;
				int cc = packet[3] & 0xFF;

				// Check if the packet contains SCTE35 payload
				if (pid == scte35_pid && (packet[3] & 0x10)) {
					// Extract SCTE35 payload
					if (packet[3] & 0x30 == 0x30) {
						adaptation_len = packet[4] + 1;
					}
					int scte35_length = packet[7 + adaptation_len];
					std::vector<uint8_t> scte35_payload(packet.begin() + 4 + adaptation_len, packet.begin() + 4 + scte35_length + 4 + adaptation_len);

					if (scte35_length != 17) {
						extractSCTEInformation(scte35_payload);
						std::vector<uint8_t> dsmcc_packet = buildDSMCCPacket(scte35_payload, version_count, packet, cont_count);
						cont_count += 1;
						cont_count &= 0x0F;
						events_replaced += 1;

						output_stream.write(reinterpret_cast<char*>(dsmcc_packet.data()), dsmcc_packet.size());
						version_count += 1;
					}
					else {
						if (!replaceNull) {
							events_notreplaced += 1;
							sendStuffedPacket(output_stream);
						}
						else {
							std::vector<uint8_t> dsmcc_packet = buildDSMCCPacket(scte35_payload, version_count, packet, cont_count);
							cont_count += 1;
							cont_count &= 0x0F;
							events_replaced += 1;
							output_stream.write(reinterpret_cast<char*>(dsmcc_packet.data()), dsmcc_packet.size());
						}
					}
				}
				else {
					output_stream.write(reinterpret_cast<char*>(packet.data()), packet.size());
				}
				packetcount += 1;
			}

			std::cout << "\nTotal SCTE Events Replaced: " << events_replaced << std::endl;
			std::cout << "Total SCTE Events Ignored: " << events_notreplaced << std::endl;
			std::cout << "SCTE to DSMCC replacement complete\n" << std::endl;
		}






		//XML EXTRACTION FUNCTIONS
	
		/**
		Function to get the XML for the stream
		Parameters:
		Null
		Outputs:
		Null
		*/
		void getXML(){
			std::string command = 'tsp -I file " + input + " -P psi -x "dataXML.xml" -d';
			system(command);
		}
		
		/**Functions NEEDED(need xml parser)
		* findPMTPID
		* findAvailablePIDs
		* replaceSCTEElement
		* addDSMCCToService
		* getSCTEPID
		* getXML
		* get_service_name
		* save_pmt_by_service_id
		* save_pat
		* serviceChoice
		*/

		
		
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
		std::string destinationFileName = "inputcopy.ts"; 
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

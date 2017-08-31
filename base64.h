#include <string>
#include <vector>
const char base64_const[] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"};

std::string base64(std::string input){
	std::string out;

	for (int i=0; i <= input.size() - 3; i+=3){
		out += base64_const[((input[i] >> 2) & 0x3f)];
		out += base64_const[((input[i] & 0x3) << 4) + ((input[i + 1] >> 4) & 0xf)];
		out += base64_const[((input[i + 1] & 0xf) << 2) + ((input[i + 2] & 0xc0) >> 6)];
		out += base64_const[((input[i + 2] & 0x3f))];
	}

	if (input.size() % 3 == 2){
		out += base64_const[((input[input.size() - 2] >> 2) & 0x3f)];
		out += base64_const[((input[input.size() - 2] & 0x3) <<4) + ((input[input.size() - 1] >> 4) & 0xf)];
		out += base64_const[((input[input.size() - 1] & 0xf) << 2)];
		out += "=";
	}

	if (input.size() % 3 == 1){
		out += base64_const[((input[input.size() - 1] >> 2) & 0x3f)];
		out += base64_const[((input[input.size() - 1] & 0x3) << 4)];
		out += "==";
	}

	return out;
};

std::string base64_encode(char input[], unsigned long size){
	std::string out;
	if(size < 3)
		return std::string();
	for(int i = 0; i <= size - 3; i += 3){
		out += base64_const[((input[i] >> 2) & 0x3f)];
		out += base64_const[((input[i] & 0x3) << 4) + ((input[i + 1] >> 4) & 0xf)];
		out += base64_const[((input[i + 1] & 0xf) << 2) + ((input[i + 2] & 0xc0) >> 6)];
		out += base64_const[((input[i + 2] & 0x3f))];
	}

	if(size % 3 == 2){
		out += base64_const[((input[size - 2] >> 2) & 0x3f)];
		out += base64_const[((input[size - 2] & 0x3) << 4) + ((input[size - 1] >> 4) & 0xf)];
		out += base64_const[((input[size - 1] & 0xf) << 2)];
		out += "=";
	}

	if(size % 3 == 1){
		out += base64_const[((input[size - 1] >> 2) & 0x3f)];
		out += base64_const[((input[size - 1] & 0x3) << 4)];
		out += "==";
	}

	return out;
};


std::string base64_decode_to_string(std::string input){
	std::string out;
	int adj_size;

	if (input[input.size()-2] == '=')
		adj_size = input.size() - 2;
	else if(input[input.size()-1]== '=')
		adj_size = input.size() - 1;
	else
		adj_size = input.size();

	char *buffer = new char[adj_size+1];
	memset(buffer,0,adj_size+1);
	for (int i=0; i < adj_size; i++){
		for (int it=0; it < 64; it++){
			if (base64_const[it] == input[i])
				buffer[i] = it;
		}
	}
	//
	for (int i=0; i < adj_size; i+=4){
		out += (buffer[i] << 2 ) + ((buffer[i+1] >> 4) & 0x3);
		out += ((buffer[i+1] & 0xf) << 4) + ((buffer[i+2] & 0x3c) >> 2);
		out += ((buffer[i+2] << 6) & 0xc0) + ((buffer[i+3] & 0x3f));
	}

	if (adj_size % 4 == 3){//1 more char
		out.erase(out.end()-1);
	}
	else if(adj_size % 4 == 2){//2 more char
		out.erase(out.end()-2, out.end());
	}

	return out;
};

std::vector<char> base64_decode_to_vector(std::string input){

	std::vector<char> out;
	int adj_size;

	if(input[input.size() - 2] == '=')
		adj_size = input.size() - 2;
	else if(input[input.size() - 1] == '=')
		adj_size = input.size() - 1;
	else
		adj_size = input.size();

	char *buffer = new char[adj_size + 1];
	memset(buffer, 0, adj_size + 1);
	for(int i = 0; i < adj_size; i++){
		for(int it = 0; it < 64; it++){
			if(base64_const[it] == input[i])
				buffer[i] = it;
		}
	}
	//
	for(int i = 0; i < adj_size; i += 4){
		out.push_back((buffer[i] << 2) + ((buffer[i + 1] >> 4) & 0x3));
		out.push_back(((buffer[i + 1] & 0xf) << 4) + ((buffer[i + 2] & 0x3c) >> 2));
		out.push_back(((buffer[i + 2] << 6) & 0xc0) + ((buffer[i + 3] & 0x3f)));
	}

	if(adj_size % 4 == 3){//1 more char
		out.erase(out.end() - 1);
	}
	else if(adj_size % 4 == 2){//2 more char
		out.erase(out.end() - 2, out.end());
	}

	return out;
};
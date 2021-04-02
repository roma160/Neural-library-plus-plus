#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define endl "\n"

using namespace std;

const char* magic_string = "\x93NUMPY";
const char* descr = "\'descr\' : \'|";
const char* shape = "\'shape\': (";
const char* separator = "\n------------------------------------------------\n";
const char header_length_size = 2;

struct npy_contents
{
	int data_start;
	stringstream data_shape;
	string data_type;
} read_npy_contents(ifstream* npy_file)
{
	npy_contents ret;

	unsigned short int header_len = 0;
	npy_file->seekg(strlen(magic_string) + 2, ios_base::beg);
	npy_file->read((char*) &header_len, header_length_size);
	
	string header(new char[header_len]);
	npy_file->read((char*)header.c_str(), header_len);

	unsigned short data_type_start = header.find(descr) + strlen(descr);
	ret.data_type = header.substr(
		data_type_start + 1,
		header.find('\'', data_type_start) - data_type_start - 1);
	
	unsigned short data_shape_start = header.find(shape) + strlen(shape);
	ret.data_shape = stringstream(
		header.substr(data_shape_start,
			header.find(')', data_shape_start) - data_shape_start)
	);

	ret.data_start = strlen(magic_string) + 2 + header_length_size + header_len;
	return ret;
}

size_t get_int(const char* int_name)
{
	size_t ret;
	char check = ' ';
	do
	{
		cout << "Please, enter the " << int_name << ": ";
		cin >> ret;
		cout << int_name << " = " << ret << ". Are you sure? (Y/N) : ";
		cin >> check;
	} while (check != 'Y');
	return ret;
}
bool get_bool(const char* message)
{
	char entered = ' ';
	while (entered != 'Y' && entered != 'N')
	{
		cout << message << " (Y/N) : ";
		cin>>entered;
	}
	return entered == 'Y';
}

char get_type_size(const string &type)
{
	int i = 1;
	while (!isdigit(type[i])) i++;
	if (i == 1) return stoi(type.substr(1));
	return stoi(type.substr(i)) / 8;
}
void* create_variable_of_type(const string &type)
{
	if (type == "u1") return new unsigned char;
	return nullptr;
}
long long int to_printable(const void* val, const string &type)
{
	if (type[0] == 'u') return *(unsigned char*) val;
	return 0;
}

void program(ifstream* input, ifstream* output, ofstream* result)
{
	//Checking magic string
	char* buff = new char[7];
	buff[6] = '\0';
	input->read(buff, 6);
	if(strcmp(buff, magic_string))
	{
		cout << "Input file wasn`t .npy format.\nExiting...";
		return;
	}
	output->read(buff, 6);
	if (strcmp(buff, magic_string))
	{
		cout << "Output file wasn`t .npy format.\nExiting...";
		return;
	}
	delete[] buff;

	npy_contents input_file_contents = read_npy_contents(input),
		output_file_contents = read_npy_contents(output);
	input->seekg(input_file_contents.data_start);
	output->seekg(output_file_contents.data_start);

	size_t data_length, input_layer_size, output_layer_size;
	cout << "Input data shape : (" << input_file_contents.data_shape.str() << ")\n";
	cout << "Output data shape : (" << output_file_contents.data_shape.str() << ")\n";
	cout << endl;
	data_length = get_int("number of train samples");
	cout << endl;
	input_layer_size = get_int("neural network input size");
	cout << endl;
	output_layer_size = get_int("neural network output size");

	// Checking input data
	/*cout << separator;
	string* input_data_type = &input_file_contents.data_type;
	void* buff_input_data = create_variable_of_type(*input_data_type);
	char input_data_size = get_type_size(*input_data_type);
	bool justify_input_data = false;
	if((*input_data_type)[0] == 'b' ||
		(*input_data_type)[0] == 'i' ||
		(*input_data_type)[0] == 'u'){
		cout << "Here are some input data numbers : \n";
		for(int i = 0; i < 300; i++)
		{
			input->read((char*)buff_input_data, input_data_size);
			cout << to_printable(buff_input_data, *input_data_type) << " ";
		}
		cout << "\n\nDo you want to justify input data, so when the data = 5, "
			"the input will be :\n(0.0) (0.0) (0.0) (0.0) (1.0)";
		justify_input_data = get_bool("");
	}*/

	//TODO: implement more flexible conversion system
	//	(now its working only for MNIST.npy train data)
	
	cout << separator << "\nThe result file header : " << endl <<
		"Input layer size : " << input_layer_size << endl <<
		"Output layer size : " << output_layer_size << endl <<
		"Number of data samples : " << data_length << endl <<
		"Writing started..." << endl;

	result->write((char*) &input_layer_size, sizeof(size_t));
	result->write((char*) &output_layer_size, sizeof(size_t));
	result->write((char*) &data_length, sizeof(size_t));

	unsigned char buff_data = 0;
	double buff_double = 0;
	input->seekg(input_file_contents.data_start);
	output->seekg(output_file_contents.data_start);
	for(size_t i = 0; i < data_length; i++)
	{
		for(size_t j = 0; j < input_layer_size; j++)
		{
			input->read((char*) &buff_data, 1);
			buff_double = buff_data / 255.0;
			result->write((char*)&buff_double, sizeof(double));
		}

		output->read((char*)&buff_data, 1);
		for(size_t j = 0; j < output_layer_size; j++)
		{
			buff_double = j == buff_data;
			result->write((char*)&buff_double, sizeof(double));
		}

		if (!(i % 1000))
			cout << "Progress : " << i << endl;
	}

	result->close();
	input->close();
	output->close();
}

void interactive_mode()
{
	string buff_string;
	ifstream input_file("");
	while (!input_file.good())
	{
		cout << "Enter input file location : ";
		getline(cin, buff_string);
		input_file.open(buff_string, ios::in | ios::binary);
	}
	ifstream output_file("");
	while (!output_file.good())
	{
		cout << "Enter output file location : ";
		getline(cin, buff_string);
		output_file.open(buff_string, ios::in | ios::binary);
	}
	
	ofstream result_file;
	buff_string = "";
	while (buff_string.empty() || !result_file.good())
	{
		cout << "Enter result file location : ";
		getline(cin, buff_string);
		if(buff_string.empty()) continue;
		result_file.open(buff_string, ios::out | ios::binary);
	}
	cout << "Starting program...\n";
	program(&input_file, &output_file, &result_file);
}

void invalid_arguments(const char* message)
{
	cout << message << "\nDo you want to start interactive mode? ";
	char entered = 'l';
	while (entered != 'Y' && entered != 'N')
	{
		cout << "(Y/N) : ";
		entered = getc(stdin);
	}
	if (entered == 'Y') interactive_mode();
	else cout << "Exiting...";
}

int main(int argc, char* argv[])
{
#ifdef _DEBUG
	argc = 7;
	argv = new char* []
	{
		"", "-i", "C:\\Users\\1\\Downloads\\mnist\\x_train.npy",
			"-o", "C:\\Users\\1\\Downloads\\mnist\\y_train.npy",
			"-r", "./res.b"
	};
#endif
	
	if (argc > 1)
	{
		if (argc == 2 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")))
			cout << "This program helps to convert .npy train data to"
			"binary file, which my library could understand." << endl <<
			"Options:" << endl <<
			"-h | --help      Prints this help." << endl <<
			"-i | --input     Defines npy file with input data." << endl <<
			"-o | --output    Defines npy file with output data." << endl <<
			"-r | --result    Defines b file with the result of this program." << endl;
		else if (argc == 5)
		{
			char* input = nullptr, * output = nullptr;
			for (int i = 1; i < 4; i += 2)
			{
				if (!strcmp(argv[i], "-i") || !strcmp(argv[i], "--input"))
					input = argv[i + 1];
				else if (!strcmp(argv[i], "-o") || !strcmp(argv[i], "--output"))
					output = argv[i + 1];
			}
			if (input != nullptr && output != nullptr)
			{
				ifstream input_file(input, ios::in | ios::binary),
					output_file(output, ios::in | ios::binary);
				if (!input_file.good())
				{
					invalid_arguments("Entered invalid input file location!");
					return 0;
				}
				if (!output_file.good())
				{
					invalid_arguments("Entered invalid output file location!");
					return 0;
				}
				cout << "Result will be saved to ./result.b...\n";
				ofstream result_file("./result.b", ios::out | ios::binary);
				cout << "Starting program...\n";
				program(&input_file, &output_file, &result_file);
			}
			else invalid_arguments("You need to specify -i and -o parameters!");
		}
		else if (argc == 7)
		{
			char* input = nullptr, * output = nullptr, * result = nullptr;
			for (int i = 1; i < 6; i += 2)
			{
				if (!strcmp(argv[i], "-i") || !strcmp(argv[i], "--input"))
					input = argv[i + 1];
				else if (!strcmp(argv[i], "-o") || !strcmp(argv[i], "--output"))
					output = argv[i + 1];
				else if (!strcmp(argv[i], "-r") || !strcmp(argv[i], "--result"))
					result = argv[i + 1];
			}
			if (input != nullptr && output != nullptr)
			{
				ifstream input_file(input, ios::in | ios::binary),
					output_file(output, ios::in | ios::binary);
				if (!input_file.good())
				{
					invalid_arguments("Entered invalid input file location!");
					return 0;
				}
				if (!output_file.good())
				{
					invalid_arguments("Entered invalid output file location!");
					return 0;
				}
				ofstream result_file;
				if (result != nullptr)
				{
					new (&result_file) ofstream(result, ios::out | ios::binary);
					if (!result_file.good())
					{
						invalid_arguments("Entered invalid result file location!");
						return 0;
					}
				}
				else
				{
					cout << "Result will be saved to ./result.b...\n";
					new (&result_file) ofstream("./result.b", ios::out | ios::binary);
				}
				cout << "Starting program...\n";
				program(&input_file, &output_file, &result_file);
			}
			else invalid_arguments("You need to specify -i and -o parameters!");
		}
		else invalid_arguments("Invalid number of parameters!");
	}
	else interactive_mode();
	return 0;
}
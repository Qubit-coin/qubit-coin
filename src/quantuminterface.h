// Cybersecurity lab
#ifndef BITCOIN_QUANTUMINTERFACE_H
#define BITCOIN_QUANTUMINTERFACE_H

#include <string>
#include <oqs/oqs.h>
#include <logging.h>


/*
	algorithm: What algorithm the instance of the class is using
	public_key_length: Bytes of the public key
	private_key_length: Bytes of the private key
	signature_length: Bytes of the signature length
	generate_keypair(): Generate the public and private keys
	get_public_key(): Get the public key in a hex string
	get_private_key(): Get the private key in a hex string
	public_key: The public key as a char array
	private_key: The private key as a char array
	sign(message): Return the signature (unsigned char*) for a message (std::string)
	verify(message, signature): Return true or false for if a message and signature are valid
*/
class QuantumSigMan {
private:
	char* algorithm_char;
	OQS_SIG *sig;

public:
	// Counts the number of instances
	// occurance_counter[0]: Allocation
	// occurance_counter[1]: Key Generation
	// occurance_counter[2]: Signing
	// occurance_counter[3]: Verifying
	static int occurance_counter[4];

	std::string algorithm;
	unsigned int public_key_length, private_key_length, signature_length;
	unsigned char *public_key, *private_key;
	
	QuantumSigMan(std::string _algorithm = "DILITHIUM_2");

	// Generate a public and private key pair
	void generate_keypair();

	// Get the generated public key
	std::string get_public_key() const;

	// Get the generated private key
	std::string get_private_key() const;

	// Sign a message, returns its signature
	unsigned char* sign(std::string message);

	// Verify a signature
	bool verify(std::string message, unsigned char* signature);

	// Given an array of bytes, convert it to a hexadecimal string
	static std::string bytes_to_hex(unsigned char* bytes, int len) {
		constexpr char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
		std::string output(len * 2, ' ');
		for (int i = 0; i < len; ++i) {
			output[2 * i] = hexmap[(bytes[i] & 0xF0) >> 4];
			output[2 * i + 1] = hexmap[bytes[i] & 0x0F];
		}
		return output;
	}
};

#endif // BITCOIN_QUANTUMINTERFACE_H

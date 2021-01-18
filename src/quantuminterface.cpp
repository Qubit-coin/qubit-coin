#include <string>
#include <oqs/oqs.h>
#include <quantuminterface.h>

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
QuantumSigMan::QuantumSigMan(std::string _algorithm) {
	algorithm = _algorithm;
	algorithm_char = const_cast<char*>(algorithm.c_str());

	OQS_randombytes_switch_algorithm(OQS_RAND_alg_system);
	sig = OQS_SIG_new(algorithm_char);

	public_key_length = sig->length_public_key;
	private_key_length = sig->length_secret_key;
	signature_length = sig->length_signature;

	public_key = (unsigned char*) malloc(public_key_length);
	private_key = (unsigned char*) malloc(private_key_length);

	LogPrint(BCLog::QUANTUM, "Quantum sigman generated for %s.\n", algorithm);
}

// Generate a public and private key pair
void QuantumSigMan::generate_keypair() {
	OQS_STATUS status = OQS_SIG_keypair(sig, public_key, private_key);
	LogPrint(BCLog::QUANTUM, "$s key pair has been generated.\n", algorithm);
	if(status != OQS_SUCCESS) return; //throw std::runtime_error("ERROR: OQS_SIG_keypair failed\n");
	return;
}

// Get the generated public key
std::string QuantumSigMan::get_public_key() const {
	return bytes_to_hex(public_key, public_key_length);
}

// Get the generated private key
std::string QuantumSigMan::get_private_key() const {
	return bytes_to_hex(private_key, private_key_length);
}

// Sign a message, returns its signature
unsigned char* QuantumSigMan::sign(std::string message) {
	unsigned char *signature = (unsigned char*) malloc(signature_length);
	unsigned int message_length = message.length();
	size_t *signature_len = (size_t*) &signature_length;
	uint8_t *message_bytes = reinterpret_cast<uint8_t*>(&message[0]);

	OQS_STATUS status = OQS_SIG_sign(sig, signature, signature_len, message_bytes, message_length, private_key);

	LogPrint(BCLog::QUANTUM, "%s message \"%s\" was signed.\n", algorithm, message);

	if (status != OQS_SUCCESS) return 0; //throw std::runtime_error("ERROR: OQS_SIG_sign failed\n");

	return signature;
}

// Verify a signature
bool QuantumSigMan::verify(std::string message, unsigned char* signature) {
	unsigned int message_length = message.length();
	//size_t *signature_len = (size_t*) &signature_length;
	uint8_t *message_bytes = reinterpret_cast<uint8_t*>(&message[0]);

	OQS_STATUS status = OQS_SIG_verify(sig, message_bytes, message_length, signature, signature_length, public_key);

	LogPrint(BCLog::QUANTUM, "%s message \"%s\" was verified.\n", algorithm, message);
	LogPrint(BCLog::QUANTUM, "%s message is %s.\n", algorithm, (status == OQS_SUCCESS ? "VALID" : "INVALID"));

	return status == OQS_SUCCESS;
}

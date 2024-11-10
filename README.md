# OP-TEE-Trusted-application - Secure Integration

This repository contains the source code and documentation for a **Trusted Application (TA)** designed to run within the **OP-TEE Trusted Execution Environment (TEE)**. The TA ensures secure data processing using **asymmetric encryption** and **trusted storage APIs**, providing a robust solution for critical operations in enterprise application integrations, particularly in **smart city environments**.

---

## Features
- **Asymmetric Encryption:** Secure data encryption and decryption using public/private keys within the TEE.
- **Trusted Storage API:** Ensures sensitive data and keys are securely stored in isolated storage.
- **Data Validation:** Performs secure validation of sensitive information (e.g., price thresholds) in the TEE.
- **Seamless Integration:** Facilitates communication between the Normal World and the Secure World using the OP-TEE framework.
- **Portability:** Supports various hardware platforms using ARM TrustZone and OP-TEE.

---

## Repository Structure
- **`eai_integration`**: Contains all the source code for the TA
   - **`host/`**: Source code for the Normal World client application.
   - **`ta/`**: Source code for the Trusted Application running in the Secure World.

---

## Getting Started

### Prerequisites
1. Set up your development environment by following the [OP-TEE documentation](https://optee.readthedocs.io/).
2. Ensure you have access to a development board or device that supports **ARM TrustZone** and **OP-TEE**.

### Building and Deploying
1. Clone this repository:
   ```bash
   git clone https://github.com/your-repository/secure-integration-ta.git
   cd secure-integration-ta
   ```
2. Build the Trusted Application and host application:
   ```
   make
   ```
3. Deploy the TA onto your target device:
   ```
   make install
   ```
### Example usage
This TA processes encrypted JSON data and performs secure validation in the TEE. For example, it validates whether a purchase price exceeds a predefined threshold, ensuring all processing remains secure.

### Example script
```
while true; do
  response=$(wget -q -O - http://example.com/api)
  echo "$response" | ./optee_example_ke_hello_world
  sleep 1
done
```
The API should return a JSON with a `price` key which the TA will validate wether or not it exceeds a certian value (currently 300), if so it will indicate to the normal world that the purchase has been validated.

---

## References

- [OP-TEE Documentation](https://optee.readthedocs.io/)
- [ARM TrustZone Overview](https://developer.arm.com/architectures/security-architectures/trustzone)

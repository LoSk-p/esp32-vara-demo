from substrateinterface import KeypairType, Keypair

seed = input("Write your account seed phrase: ")
keypair = Keypair.create_from_mnemonic(seed, crypto_type=KeypairType.ED25519)
print(f"Private key: {keypair.private_key.hex()}")
print(f"Public key: {keypair.public_key.hex()}")
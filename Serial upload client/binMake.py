FILE_SIZE_BYTES = 32768
FILE_NAME = "code.bin"


def main():
    data = bytearray()
    for i in range(FILE_SIZE_BYTES):
        data.append(0xEA)
    
    with open(FILE_NAME, "wb") as binFile:
        binFile.write(data)
    
if __name__ == "__main__":
    main()
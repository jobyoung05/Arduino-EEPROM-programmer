"""
Binary file generator for use with programmer.py
Copyright (C) 2024 Job Young

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

"""

FILE_SIZE_BYTES = 32768
FILE_NAME = "code.bin"


def main():
    """ Generate a binary file of repeated bytes (0xEA) for validation
        and testing of programmer.py
    """
    data = bytearray()
    for i in range(FILE_SIZE_BYTES):
        data.append(0xEA)
    
    with open(FILE_NAME, "wb") as binFile:
        binFile.write(data)
    
if __name__ == "__main__":
    main()
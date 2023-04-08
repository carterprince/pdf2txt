# pdf2txt

A command-line utility to extract text from PDF files using the Poppler library.

## Dependencies

- Poppler with glib bindings
- OpenMP

## Installation

To install the program, run:

```bash
sudo make install
```

This will install the executable to `/usr/local/bin` (or the path specified in the Makefile).

## Usage

To extract text from a PDF file, run:

```bash
pdf2txt [--stdout] <path_to_pdf>
```

By default, the program will create a text file with the same name as the PDF file in the current directory. If you pass the `--stdout` option, the extracted text will be printed to the standard output instead.

## License

This project is released under the [GNU General Public License v3.0](LICENSE).

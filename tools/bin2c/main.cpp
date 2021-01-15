// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#define KB_TOOL_ONLY

#include <kb/stream.h>
#include <kbextra/cliargs.h>

#include "kb/alloc.cpp"
#include "kb/crt.cpp"
#include "kb/math.cpp"

#include "kbextra/cliargs.cpp"

#include "platform/platform_rwops_stdio.cpp"

#define EXIT_FAIL     1
#define EXIT_SUCCESS  0

#define MAX_COL       80
#define BUF_SIZE      512

void print_help(const char* error = nullptr) {
  if (error != nullptr) printf("%s\n", error);

  printf(
    "Kimberlite bin2c\n"
    "\tUsage: bin2c --input <in> --output <out> --name <name>\n"
  );
}

int main(int argc, const char* argv[]) {  
  kb_cli_args args;  
  kb_cliargs_init(&args, argc, argv);
    
  if (kb_cliargs_has(&args, "help")) {
    print_help();
    return EXIT_FAIL;
  } 

  const char* in_filepath = nullptr;
  kb_cliargs_get_str(&args, &in_filepath, "input");
  if (in_filepath == nullptr) {
    print_help("Please specify input file with --input");
    return EXIT_FAIL;
  }

  const char* out_filepath;
  kb_cliargs_get_str(&args, &out_filepath, "output");
  if (out_filepath == nullptr) {
    print_help("Please specify output file with --output");
    return EXIT_FAIL;
  }

  const char* arr_name;
  kb_cliargs_get_str(&args, &arr_name, "name");
  if (arr_name == nullptr) {
    print_help("Please specify array name --name");
    return EXIT_FAIL;
  }

  kb_stream* rwops_in = kb_stream_open_file(in_filepath, KB_FILE_MODE_READ);
  if (!rwops_in) {
    print_help("Unable to open input file");
    return EXIT_FAIL;
  }
  
  kb_stream* rwops_out = kb_stream_open_file(out_filepath, KB_FILE_MODE_WRITE);
  if (!rwops_out) {
    kb_stream_close(rwops_in);
    print_help("Unable to open output file");
    return EXIT_FAIL;
  }

  char buf[BUF_SIZE];
  int count = 0;

  // Write header
  count = kb_snprintf(buf, BUF_SIZE, "static const uint8_t %s[] = {\n  ", arr_name);
  kb_stream_write(rwops_out, buf, 1, count);

  // Dump file
  unsigned char c;
  uint32_t col = 0;
  while (kb_stream_read(rwops_in, &c, 1, 1) == 1) {
    count = kb_snprintf(buf, BUF_SIZE, "0x%.2X, ", (int) c);

    kb_stream_write(rwops_out, buf, 1, count);

    col += count;
    if (col >= MAX_COL) {
      col = 0;
      kb_stream_write(rwops_out, "\n  ", 1, 3);
    }
  }

  // Ending
  if (col != 0) kb_stream_write(rwops_out, "\n", 1, 1);
  kb_stream_write(rwops_out, "};\n", 1, 3);

  // Cleanup
  kb_stream_close(rwops_in);
  kb_stream_close(rwops_out);

	return EXIT_SUCCESS;
}

// cnfReader.h
#ifndef CNF_READER_H
#define CNF_READER_H

// Returns malloc'ed string (caller must free). On error returns NULL.
char *cnfToInfix(const char *filename);

#endif

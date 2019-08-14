extern void parse(char *filename);

int main(int argc, char *argv[])
{
    if (argc == 2)
        parse(argv[1]);
}

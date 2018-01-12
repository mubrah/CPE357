int main(int argc, char *argv[]) {
	for (int i; i < argc; i++) {
		if (argv[i][0] == '-') {
			printf(argv[i]);
		}
	}
	return 0;
}
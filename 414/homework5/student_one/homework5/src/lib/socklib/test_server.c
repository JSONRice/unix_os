main()
{
	int fd;

	fd = setup_server (7774);
	sleep (10);
	close (fd);
	exit (0);
}


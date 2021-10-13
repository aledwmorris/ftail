# ftail
trivial re-implementation of "tail -F"

Running under FreeBSD-13.0-RELEASE, I found that "tail -F" on a log file rotated by newsyslog(8) didn't work.  Specifically it didn't seem to notice that the log file had been rotated and so didn't re-open the file.  Since this is the main benefit of "-F" over "-f" I'd say this is a bug.

Both newsyslog(8) and tail(1) are relatively complex programs, using kernel messages, memory mapping etc. to achieve their purpose, which would make troubleshooting them a laborious and time consuming process.

Instead I wrote this very simple program which does all I needed - print data appended to a file and keep doing so even if the file is rotated.

The watched file is polled once a second to see if there is any data available.  Only if there is no data will it check to see if the file has been rotated.  This code would be cleaner if C supported a "while...else" syntax; without it I have had to use a boolean flag.

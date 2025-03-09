SHELL := /bin/bash
DESTINATION = backupdir
SOURCE = src
INTERVAL_SEC = 3
MAX_BACKUPS = 3
$(DESTINATION):
	@if [ ! -d "$(DESTINATION)" ];
	then \
		mkdir -p "$(DESTINATION)"; \
	fi
backup:
	@./backup.sh $(SOURCE) $(DESTINATION) $(INTERVAL_SEC) $(MAX_BACKUPS)   
	
restore:
	./restore.sh $(SOURCE) $(DESTINATION)


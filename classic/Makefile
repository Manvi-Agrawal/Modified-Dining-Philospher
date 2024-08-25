TARGET_ORDER = strict_order.out
TARGET_SEMAPHORE = semaphore.out


SOURCE_ORDER = strict_order.c
SOURCE_SEMAPHORE = semaphore.c

all: $(TARGET_ORDER) $(TARGET_SEMAPHORE)

$(TARGET_ORDER):
	gcc $(SOURCE_ORDER) -o $(TARGET_ORDER)

$(TARGET_SEMAPHORE):
	gcc $(SOURCE_SEMAPHORE) -o $(TARGET_SEMAPHORE)


runStrictOrder:
	./$(TARGET_ORDER)
runSemaphore:
	./$(TARGET_SEMAPHORE)


clean:
	rm -rf $(TARGET_ORDER) $(TARGET_SEMAPHORE)
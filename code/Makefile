all: fork loop context_switch function_overhead mem msmt page_fault syscall
	echo "Done!"

clean:
	rm *.8 fork loop context_switch function_overhead mem msmt page_fault syscall

syscall:
	8c syscall.c
	8l -o syscall syscall.8

page_fault:
	8c page_fault.c
	8l -o page_fault page_fault.8

fork:
	8c fork.c
	8l -o fork fork.8

loop:
	8c loop.c
	8l -o loop loop.8

context_switch:
	8c context_switch.c
	8l -o context_switch context_switch.8

function_overhead:
	8c function_overhead.c
	8l -o function_overhead function_overhead.8

mem:
	8c mem.c
	8l -o mem mem.8

msmt:
	8c msmt.c
	8l -o msmt msmt.8

// SPDX-License-Identifier: BSD-3-Clause

#include <internal/mm/mem_list.h>
#include <internal/types.h>
#include <internal/essentials.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

void *malloc(size_t size)
{
	/* TODO: Implement malloc(). */
	if (size == 0)
		return NULL;

	void *block = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (block == MAP_FAILED)
		return NULL;

	if (mem_list_add(block, size) != 0)
	{
		munmap(block, size);
		return NULL;
	}

	return block;
}

void *calloc(size_t nmemb, size_t size)
{
	/* TODO: Implement calloc(). */
	if (nmemb == 0 || size == 0)
		return NULL;

	void *block = mmap(NULL, nmemb * size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (block == MAP_FAILED)
		return NULL;

	if (mem_list_add(block, nmemb * size) != 0)
	{
		munmap(block, nmemb * size);
		return NULL;
	}

	memset(block, 0, nmemb * size);

	return block;
}

void free(void *ptr)
{
	/* TODO: Implement free(). */
	if (ptr == NULL)
		return;

	struct mem_list *block = mem_list_find(ptr);
	if (!block)
		return;

	munmap(ptr, block->len);
	mem_list_del(ptr);
}

void *realloc(void *ptr, size_t size)
{
	/* TODO: Implement realloc(). */
	if (ptr == NULL)
		return malloc(size);

	if (size == 0)
	{
		free(ptr);
		return NULL;
	}

	struct mem_list *block = mem_list_find(ptr);
	if (!block)
		return ptr;

	void *new_block = mremap(ptr, block->len, size, MREMAP_MAYMOVE);
	if (new_block == MAP_FAILED)
		return ptr;

	if (new_block != ptr)
	{
		size_t copy_size;
		if (block->len < size)
			copy_size = block->len;
		else
			copy_size = size;

		memcpy(new_block, ptr, copy_size);
		munmap(ptr, block->len);
	}

	block->start = new_block;
	block->len = size;

	return new_block;
}

void *reallocarray(void *ptr, size_t nmemb, size_t size)
{
	/* TODO: Implement reallocarray(). */
	if (nmemb == 0 || size == 0)
	{
		free(ptr);
		return NULL;
	}

	if (nmemb * size > (size_t)-1)
	{
		errno = ENOMEM;
		return NULL;
	}

	return realloc(ptr, nmemb * size);
}

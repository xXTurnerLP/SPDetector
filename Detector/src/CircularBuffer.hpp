#pragma once

#include <stdlib.h>
#include <cstring>
#include <cstdarg>
#include <list>

template<typename T>
class CircularBuffer
{
public:
	/**
	 * Create circular buffer with set size.
	 *
	 * \param size - Max size
	 */
	CircularBuffer<T>(const size_t size) noexcept : buffer(new T[size]), buffer_size(size), selector(0)
	{
		memset(buffer, 0, size);
	}

	/**
	 * Create static circular buffer with set size. Used for cross referencing.
	 *
	 * \param size - Max size
	 * \param ... - Each element
	 */
	CircularBuffer<T>(const char* CONSTRUCTOR_DUPLICATE_DUMMY_BYPASS, const size_t size, ...) noexcept : buffer(new T[size]), buffer_size(size), selector(0)
	{
		va_list args;
		va_start(args, size);

		for (size_t i = 0; i < size; i++)
		{
			buffer[i] = va_arg(args, T);
		}

		va_end(args);
	}

	~CircularBuffer() noexcept
	{
		delete[] buffer;
	}

	/**
	 * Insert element at latest position (rotating/overwriting as needed).
	 *
	 * \param element - Element to insert. Should be of type T
	 */
	void Insert(T& element) noexcept
	{
		buffer[selector++] = element;

		if (selector == buffer_size)
			selector = 0;
	}

	/**
	 * Peek latest element (the one that was written when used with latest Insert).
	 *
	 * \return Latest element T
	 */
	const T Peek() noexcept
	{
		if (selector == 0)
			return buffer[buffer_size - 1];
		else
			return buffer[selector - 1];
	}

	/**
	 * Compare 2 buffers.
	 *
	 * \param other - Other buffer to compare aginst.
	 * \return true if they are the same, false if they are with different sizes or are different
	 */
	bool Compare(CircularBuffer& other) noexcept
	{
		if (buffer_size != other.buffer_size)
			return false;

		int offset = calculateOffset(buffer_size, buffer, other.buffer);

		if (offset == -1)
			return false;

		for (size_t i = 0; i < buffer_size; i++)
		{
			if (buffer[i] != other.buffer[offset++])
			{
				return false;
			}

			if (offset == buffer_size) // prevent buffer overflow
				offset = 0;
		}

		return true;
	}

public:
	// Copy
	CircularBuffer& operator=(CircularBuffer& other) noexcept
	{
		if (buffer_size == other.buffer_size)
		{
			memcpy(buffer, other.buffer, buffer_size);
		}
		else
		{
			delete[] buffer;
			buffer = new T[other.buffer_size];
			buffer_size = other.buffer_size;
			memcpy(buffer, other.buffer, buffer_size);
		}

		selector = other.selector;

		return *this;
	}

private:
	/**
	 * Calculate the offset, because its rotating buffer.
	 *
	 * \param buffer_size - Size of buffer, has to be the same for both buffers
	 * \param buffer_local - Buffer1, local means it will be indexed at 0
	 * \param buffer_foreign - Buffer2, foreign means it will be indexed at the calculated offset. (buffer_local[i] == bufer_foreign[i + offset] will return true)
	 */
	int calculateOffset(size_t buffer_size, T* buffer_local, T* buffer_foreign) noexcept
	{
		std::list<size_t> offset;

		for (size_t i = 0; i < buffer_size; i++)
		{
			if (buffer_local[0] == buffer_foreign[i])
			{
				offset.push_back(i);
			}
		}

		size_t offsets_count = offset.size();

		if (offsets_count == 1)
			return offset.back();

		for (size_t it = 0; it < offsets_count; it++)
		{
			size_t matches = 0;
			size_t offset_i_original = offset.back(); offset.pop_back();
			size_t offset_i = offset_i_original;

			for (size_t i = 0; i < buffer_size; i++)
			{
				if (buffer_local[i] == buffer_foreign[offset_i++])
				{
					matches++;
				}

				if (offset_i == buffer_size) // prevent buffer overflow
					offset_i = 0;
			}

			if (matches == buffer_size)
				return offset_i_original; // doing this to avoid recalculating offset since we are modifying it
		}

		return -1; // not the same buffers
	}

private:
	T* buffer;
	size_t buffer_size;
	size_t selector;
};


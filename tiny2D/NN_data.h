#pragma once

namespace NN
{
	struct Data
	{
		int n_input;
		int n_hidden;
		int n_output;
		double *data;
		int total_size;
		int single_network_size;
	};

	namespace internal
	{
		struct Temperature
		{
			double temperature;
			double decay;
			struct
			{
				int n_temperature_repeat;
				int current_repeat;
			} internal;
		};
	}

	namespace Solution
	{
		struct Solution
		{
			Data current;
			Data tmp;
			internal::Temperature temperature;

			double modify_range;
			int n_modify_count;
		};
	}
}
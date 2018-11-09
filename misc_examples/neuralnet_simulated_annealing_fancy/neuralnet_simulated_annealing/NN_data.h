#pragma once

namespace NN
{
	
	namespace internal
	{
		struct NN
		{
			int n_input;
			int n_hidden;
			double *data;
			int total_size;
			int bias_offset;
			int hidden_to_output_offset;
			//input_to_hidden_weights = offset=0, size=n_input*n_hidden
			//hidden_unit_biases = offset=n_input*n_hidden, size=n_hidden
			//hidden_to_output_weights = offset=n_input*n_hidden+n_hidden, size=n_hidden
		};

		struct Evolve_Out
		{
			double out0;
			double out1;
		};

		struct Evolve_In
		{
			double fitness0;
			double fitness1;
		};

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


	struct Solution
	{
		internal::NN current;
		internal::NN tmp;
		internal::Temperature temperature;

		double modify_range;
		int n_modify_count;
	};

}
// Includes
#include "LightBulb/LightBulbPrec.hpp"
#include "LightBulb/Teaching/AbstractTeachingLesson.hpp"
#include "LightBulb/NeuralNetwork/AbstractNeuralNetwork.hpp"
#include "LightBulb/NetworkTopology/AbstractNetworkTopology.hpp"
#include "LightBulb/Teaching/TeachingInput.hpp"
// Library includes
#include "LightBulb/NeuronDescription/NeuronDescription.hpp"
#include "LightBulb/LinearAlgebra/KernelHelper.hpp"

namespace LightBulb
{
	const Vector<>& AbstractTeachingLesson::tryLesson(AbstractNeuralNetwork& neuralNetwork, const AbstractActivationOrder& activationOrder) const
	{
		if (isCalculatorType(CT_GPU))
		{
			if (teachingPatternVector.getViennaclValue().size() != getTeachingPattern().size() + neuralNetwork.getNetworkTopology().usesBiasNeuron()) {
				teachingPatternVector.getViennaclValueForEditing().resize(getTeachingPattern().size() + neuralNetwork.getNetworkTopology().usesBiasNeuron());
				viennacl::copy(getTeachingPattern().begin(), getTeachingPattern().end(), teachingPatternVector.getViennaclValueForEditing().begin());
				if (neuralNetwork.getNetworkTopology().usesBiasNeuron())
					teachingPatternVector.getViennaclValueForEditing()[getTeachingPattern().size()] = 1;
			}
		}
		else
		{
			if (teachingPatternVector.getEigenValue().size() != getTeachingPattern().size() + neuralNetwork.getNetworkTopology().usesBiasNeuron()) {
				teachingPatternVector.getEigenValueForEditing().resize(getTeachingPattern().size() + neuralNetwork.getNetworkTopology().usesBiasNeuron());
				for (int i = 0; i < getTeachingPattern().size(); i++)
					teachingPatternVector.getEigenValueForEditing()(i) = getTeachingPattern()[i];
				if (neuralNetwork.getNetworkTopology().usesBiasNeuron())
					teachingPatternVector.getEigenValueForEditing()(getTeachingPattern().size()) = 1;
			}
		}

		return neuralNetwork.calculateWithoutOutputCopy(teachingPatternVector, activationOrder);
	}

	const Vector<>& AbstractTeachingLesson::getErrorVector(AbstractNeuralNetwork& neuralNetwork, const AbstractActivationOrder& activationOrder, bool clipError) const
	{
		// Try the lesson and extract the output
		const Vector<>& outputVector = tryLesson(neuralNetwork, activationOrder);
		getErrorVectorFromOutputVector(outputVector, neuralNetwork, clipError);

		return errorVector;
	}


	void AbstractTeachingLesson::getErrorVectorFromOutputVector(const Vector<>& outputVector, AbstractNeuralNetwork& neuralNetwork, bool clipError) const
	{
		// Get the teachingInput
		const TeachingInput<double>& teachingInput = getTeachingInput(neuralNetwork.getNetworkTopology().getOutputNeuronDescription().getActivationFunction());

		// Create the errorVector
		//std::unique_ptr<Vector> errorVector(new Vector(teachingInput.getDimension()));

		// Calculate the error values (expected value - real value)
		if (isCalculatorType(CT_GPU))
		{
			if (teachingInputVector.getViennaclValue().size() != teachingInput.size()) {
				teachingInputVector.getViennaclValueForEditing().resize(teachingInput.size());
				errorVector.getViennaclValueForEditing().resize(teachingInput.size());

				auto teachingInputRealVector = teachingInput.getRealVector();
				viennacl::copy(teachingInputRealVector.begin(), teachingInputRealVector.end(), teachingInputVector.getViennaclValueForEditing().begin());
			}

			
			calcErrorVector(errorVector.getViennaclValueForEditing(), teachingInputVector.getViennaclValue(), outputVector.getViennaclValue());
		}
		else
		{
			if (errorVector.getEigenValue().size() != teachingInput.size()) {
				errorVector.getEigenValueForEditing().resize(teachingInput.size());
			}

			std::vector<double> output(neuralNetwork.getNetworkTopology().getOutputSize());

			output.assign(outputVector.getEigenValue().data(), outputVector.getEigenValue().data() + output.size());

			for (int i = 0; i < teachingInput.getDimension(); i++)
			{
				if (teachingInput.exists(i))
				{
					errorVector.getEigenValueForEditing()[i] = teachingInput.get(i) - output[i];
					if (clipError)
						errorVector.getEigenValueForEditing()[i] = std::max(-1.0f, std::min(1.0f, errorVector.getEigenValue()[i]));
				}
				else
					errorVector.getEigenValueForEditing()[i] = 0;
			}
		}
	}

	void AbstractTeachingLesson::calcErrorVector(viennacl::vector<float>& errorVector, const viennacl::vector<float>& teachingInput, const viennacl::vector<float>& outputVector) const
	{
		static viennacl::ocl::kernel& kernel = getKernel("teaching_lesson", "calc_error_vector", "teaching_lesson.cl");

		viennacl::ocl::enqueue(kernel(
			viennacl::traits::opencl_handle(errorVector),
			cl_uint(viennacl::traits::start(errorVector)),
			cl_uint(viennacl::traits::stride(errorVector)),
			cl_uint(viennacl::traits::size(errorVector)),

			viennacl::traits::opencl_handle(teachingInput),
			cl_uint(viennacl::traits::start(teachingInput)),
			cl_uint(viennacl::traits::stride(teachingInput)),

			viennacl::traits::opencl_handle(outputVector),
			cl_uint(viennacl::traits::start(outputVector)),
			cl_uint(viennacl::traits::stride(outputVector))
		));
	}


	double AbstractTeachingLesson::getSpecificError(AbstractNeuralNetwork& neuralNetwork, const AbstractActivationOrder& activationOrder, bool clipError) const
	{
		// Calculate the errorVector
		getErrorVector(neuralNetwork, activationOrder, clipError);

		double specificError = 0;

		if (isCalculatorType(CT_GPU))
		{
			calcSpecificError(specificErrorScalar.getViennaclValueForEditing(), errorVector.getViennaclValueForEditing());
			specificError = specificErrorScalar.getViennaclValue();
		}
		else
		{
			// Add the square of every errorValue in the errorVector
			for (int i = 0; i < errorVector.getEigenValue().rows(); i++)
			{
				specificError += pow(errorVector.getEigenValue()(i), 2.0);
			}

			// Divide the specific error by two
			specificError /= 2;
		}

		return specificError;
	}

	const Vector<>& AbstractTeachingLesson::getTeachingPatternVector() const
	{
		return teachingPatternVector;
	}


	void AbstractTeachingLesson::calcSpecificError(viennacl::scalar<float>& specificError, viennacl::vector<float>& errorVector) const
	{
		static viennacl::ocl::kernel& kernel = getKernel("teaching_lesson", "calc_specific_error", "teaching_lesson.cl");

		viennacl::ocl::enqueue(kernel(
			viennacl::traits::opencl_handle(errorVector),
			cl_uint(viennacl::traits::start(errorVector)),
			cl_uint(viennacl::traits::stride(errorVector)),
			cl_uint(viennacl::traits::size(errorVector)),

			viennacl::traits::opencl_handle(specificError)
		));
	}
}

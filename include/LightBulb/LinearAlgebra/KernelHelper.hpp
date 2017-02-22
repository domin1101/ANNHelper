#pragma once

#ifndef _KERNELHELPER_H_
#define _KERNELHELPER_H_

#include <viennacl/ocl/kernel.hpp>
#include <viennacl/ocl/context.hpp>
#include <viennacl/traits/start.hpp>
#include <viennacl/traits/stride.hpp>
#include <viennacl/traits/size.hpp>
#include <viennacl/traits/handle.hpp>
#include <viennacl/matrix.hpp>

/**
 * \brief Returns the kernel with the given name.
 * \detail If it does not exist yet, it will be compiled from the file with the given path.
 * \param programName The name of the program which contains the kernel.
 * \param kernelName The name of the kernel.
 * \param fileName The path of the file which contains the program/kernel code.
 * \return The kernel.
 */
inline viennacl::ocl::kernel& getKernel(std::string programName, std::string kernelName, std::string fileName)
{
	static std::map<std::string, bool> init;
	if (!init[programName]) {
		std::ifstream ifs("cl/" + fileName);
		std::string code((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

		viennacl::ocl::current_context().add_program(code, programName);
		init[programName] = true;
	}

	return viennacl::ocl::current_context().get_program(programName).get_kernel(kernelName);
}
/**
 * \brief Executes an abstract vector cell wise assign kernel.
 * \param kernel The kernel to use.
 * \param in The vector to use as input.
 * \param out THe vector where to store the output.
 */
inline void executeVectorAssignKernel(viennacl::ocl::kernel& kernel, const viennacl::vector_base<float>& in, viennacl::vector_base<float>& out)
{
	viennacl::ocl::packed_cl_uint size_vec1;
	size_vec1.start = cl_uint(viennacl::traits::start(out));
	size_vec1.stride = cl_uint(viennacl::traits::stride(out));
	size_vec1.size = cl_uint(viennacl::traits::size(out));
	size_vec1.internal_size = cl_uint(viennacl::traits::internal_size(out));

	viennacl::ocl::packed_cl_uint size_vec2;
	size_vec2.start = cl_uint(viennacl::traits::start(in));
	size_vec2.stride = cl_uint(viennacl::traits::stride(in));
	size_vec2.size = cl_uint(viennacl::traits::size(in));
	size_vec2.internal_size = cl_uint(viennacl::traits::internal_size(in));

	viennacl::ocl::enqueue(kernel(viennacl::traits::opencl_handle(out),
		size_vec1,
		viennacl::traits::opencl_handle(in),
		size_vec2)
	);
}
/**
 * \brief Copies the given matrix to the specified tensor area.
 * \param tensor The tensor to use.
 * \param matrix The matrix to copy.
 * \param area The area index.
 */
inline void copyMatrixToTensorArea(viennacl::matrix_base<float>& tensor, const viennacl::matrix_base<float>& matrix, int area)
{
	static viennacl::ocl::kernel& kernel = getKernel("standard_operations", "copy_matrix_to_tensor_area", "standard_operations.cl");

	viennacl::ocl::enqueue(kernel(
		viennacl::traits::opencl_handle(matrix),
		viennacl::traits::size1(matrix),
		viennacl::traits::size2(matrix),
		viennacl::traits::internal_size2(matrix),
		viennacl::traits::opencl_handle(tensor),
		viennacl::traits::internal_size2(tensor),
		cl_uint(area)
	));
}
/**
* \brief Copies the given matrix to the specified tensor area.
* \param tensor The tensor to use.
* \param matrix The matrix to copy.
* \param area The area index.
*/
inline void copyMatrixToTensorArea(viennacl::matrix_base<float>& tensor, const viennacl::matrix_base<float>& matrix, const viennacl::scalar<int>& area)
{
	static viennacl::ocl::kernel& kernel = getKernel("standard_operations", "copy_matrix_to_tensor_area_scalar", "standard_operations.cl");

	viennacl::ocl::enqueue(kernel(
		viennacl::traits::opencl_handle(matrix),
		viennacl::traits::size1(matrix),
		viennacl::traits::size2(matrix),
		viennacl::traits::internal_size2(matrix),
		viennacl::traits::opencl_handle(tensor),
		viennacl::traits::internal_size2(tensor),
		viennacl::traits::opencl_handle(area)
	));
}
/**
* \brief Copies the given vector to the specified matrix column.
* \param matrix The matrix to use.
* \param vector The vector to copy.
* \param column The column index.
*/
inline void copyVectorToMatrixCol(viennacl::matrix_base<float>& matrix, const viennacl::vector_base<float>& vector, int column)
{
	static viennacl::ocl::kernel& kernel = getKernel("standard_operations", "copy_vector_to_matrix_col", "standard_operations.cl");

	viennacl::ocl::enqueue(kernel(
		viennacl::traits::opencl_handle(vector),
		viennacl::traits::size(vector),
		viennacl::traits::opencl_handle(matrix),
		viennacl::traits::internal_size2(matrix),
		cl_uint(column)
	));
}
/**
* \brief Copies the given vector to the specified matrix column.
* \param matrix The matrix to use.
* \param vector The vector to copy.
* \param column The column index.
*/
inline void copyVectorToMatrixCol(viennacl::matrix_base<float>& matrix, const viennacl::vector_base<float>& vector, const viennacl::scalar<int>& column)
{
	static viennacl::ocl::kernel& kernel = getKernel("standard_operations", "copy_vector_to_matrix_col_scalar", "standard_operations.cl");

	viennacl::ocl::enqueue(kernel(
		viennacl::traits::opencl_handle(vector),
		viennacl::traits::size(vector),
		viennacl::traits::opencl_handle(matrix),
		viennacl::traits::internal_size2(matrix),
		viennacl::traits::opencl_handle(column)
	));
}
/**
* \brief Copies the given scalar to the specified vector element.
* \param vector The vector to use.
* \param scalar The scalar to copy.
* \param element The element index.
*/
inline void copyScalarToVectorElement(viennacl::vector_base<float>& vector, const viennacl::scalar<float>& scalar, int element)
{
	static viennacl::ocl::kernel& kernel = getKernel("standard_operations", "copy_scalar_to_vector_elem", "standard_operations.cl");

	viennacl::ocl::enqueue(kernel(
		viennacl::traits::opencl_handle(vector),
		viennacl::traits::opencl_handle(scalar),
		cl_uint(element)
	));
}
/**
* \brief Copies the given scalar to the specified vector element.
* \param vector The vector to use.
* \param scalar The scalar to copy.
* \param element The element index.
*/
inline void copyScalarToVectorElement(viennacl::vector_base<float>& vector, const viennacl::scalar<float>& scalar, const viennacl::scalar<int>& element)
{
	static viennacl::ocl::kernel& kernel = getKernel("standard_operations", "copy_scalar_to_vector_elem_scalar", "standard_operations.cl");

	viennacl::ocl::enqueue(kernel(
		viennacl::traits::opencl_handle(vector),
		viennacl::traits::opencl_handle(scalar),
		viennacl::traits::opencl_handle(element)
	));
}
/**
* \brief Copies the given scalar to the specified char vector element.
* \param vector The vector to use.
* \param scalar The scalar to copy.
* \param element The element index.
*/
inline void copyScalarToVectorElement(viennacl::vector_base<char>& vector, const viennacl::scalar<char>& scalar, int element)
{
	static viennacl::ocl::kernel& kernel = getKernel("standard_operations", "copy_scalar_to_vector_elem_char", "standard_operations.cl");

	viennacl::ocl::enqueue(kernel(
		viennacl::traits::opencl_handle(vector),
		viennacl::traits::opencl_handle(scalar),
		cl_uint(element)
	));
}
/**
* \brief Copies the given scalar to the specified char vector element.
* \param vector The vector to use.
* \param scalar The scalar to copy.
* \param element The element index.
*/
inline void copyScalarToVectorElement(viennacl::vector_base<char>& vector, const viennacl::scalar<char>& scalar, const viennacl::scalar<int>& element)
{
	static viennacl::ocl::kernel& kernel = getKernel("standard_operations", "copy_scalar_to_vector_elem_char_scalar", "standard_operations.cl");

	viennacl::ocl::enqueue(kernel(
		viennacl::traits::opencl_handle(vector),
		viennacl::traits::opencl_handle(scalar),
		viennacl::traits::opencl_handle(element)
	));
}

#endif

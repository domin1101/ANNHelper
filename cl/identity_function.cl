
__kernel void identity_deriv_assign(
		__global float * vec1,
		uint4 size1,
		__global float * vec2,
		uint4 size2) {
	for (unsigned int i = get_global_id(0); i < size1.z; i += get_global_size(0)) {
		vec1[i*size1.y + size1.x] = 1;
	}
}
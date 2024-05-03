#pragma once

#define sfbxPrint(...) printf(__VA_ARGS__)

namespace sfbx {

// escape forbidden characters (e.g. " -> &quot;). return false if no escape is needed.
bool Escape(std::string &v);
std::string Base64Encode(span<char> src);

struct JointWeights;
struct JointMatrices;
bool DeformPoints(span<float3> dst, const JointWeights &jw, const JointMatrices &jm, span<float3> src);
bool DeformVectors(span<float3> dst, const JointWeights &jw, const JointMatrices &jm, span<float3> src);

} // namespace sfbx

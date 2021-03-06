/* This file is part of the Spring engine (GPL v2 or later), see LICENSE.html */

#include "MatrixState.hpp"
#include "Rendering/GL/myGL.h"
#include "System/MainDefines.h"
#include "System/MathConstants.h"
#include "System/SpringMath.h"

static              GL::MatrixState  glMatrixStates[2];
static _threadlocal GL::MatrixState* glMatrixState = nullptr;

GL::MatrixState* GL::SetMatrixStatePointer(bool mainThread) { return (glMatrixState = &glMatrixStates[mainThread]); }
GL::MatrixState* GL::GetMatrixStatePointer(               ) { return (glMatrixState                              ); }



const CMatrix44f& GL::MatrixState::Top(unsigned int mode) const {
	#ifdef CHECK_MATRIX_STACK_UNDERFLOW
	if (stacks[mode].empty()) {
		assert(false);
		return dummy;
	}
	#endif
	return (stacks[mode].back());
}

CMatrix44f& GL::MatrixState::Top(unsigned int mode) {
	#ifdef CHECK_MATRIX_STACK_UNDERFLOW
	if (stacks[mode].empty()) {
		assert(false);
		return dummy;
	}
	#endif
	return (stacks[mode].back());
}

void GL::MatrixState::Push(const CMatrix44f& m) { stack->push_back(m); }
void GL::MatrixState::Push() { Push(Top()); }
void GL::MatrixState::Pop() {
	#ifdef CHECK_MATRIX_STACK_UNDERFLOW
	if (stack->empty()) {
		assert(false);
		return;
	}
	#endif
	stack->pop_back();
}


void GL::MatrixState::Mult(const CMatrix44f& m) { Top() = Top() * m; }
void GL::MatrixState::Load(const CMatrix44f& m) { Top() = m; }

void GL::MatrixState::Translate(const float3& v) { Load((Top()).Translate(v)); }
void GL::MatrixState::Translate(float x, float y, float z) { Translate({x, y, z}); }

void GL::MatrixState::Scale(const float3& s) { Load((Top()).Scale(s)); }
void GL::MatrixState::Scale(float x, float y, float z) { Scale({x, y, z}); }

// FIXME: broken
void GL::MatrixState::Rotate(float a, float x, float y, float z) { Load((Top()).Rotate(-a * math::DEG_TO_RAD, {x, y, z})); }

void GL::MatrixState::RotateX(float a) { Load((Top()).RotateX(-a * math::DEG_TO_RAD)); }
void GL::MatrixState::RotateY(float a) { Load((Top()).RotateY(-a * math::DEG_TO_RAD)); }
void GL::MatrixState::RotateZ(float a) { Load((Top()).RotateZ(-a * math::DEG_TO_RAD)); }




#if 0
void GL::MatrixMode(unsigned int glMode) {
	switch (glMode) {
		case GL_MODELVIEW : { glMatrixState->SetMode(0); } break;
		case GL_PROJECTION: { glMatrixState->SetMode(1); } break;
		case GL_TEXTURE   : { glMatrixState->SetMode(2); } break;
		default           : {             assert(false); } break;
	}
}
#else
void GL::MatrixMode(unsigned int glMode) { glMatrixMode(glMode); }
#endif

#if 0
int GL::GetMatrixMode() {
	switch (glMatrixState->GetMode()) {
		case  0: { return GL_MODELVIEW ; } break;
		case  1: { return GL_PROJECTION; } break;
		case  2: { return GL_TEXTURE   ; } break;
		default: {        assert(false); } break;
	}

	return 0;
}
#else
int GL::GetMatrixMode() {
	int matrixMode = 0;
	glGetIntegerv(GL_MATRIX_MODE, &matrixMode);
	return matrixMode;
}
#endif

const CMatrix44f& GL::GetMatrix() { return (glMatrixState->Top()); }
const CMatrix44f& GL::GetMatrix(unsigned int glMode) {
	switch (glMode) {
		case GL_MODELVIEW : { return (glMatrixState->Top(0)); } break;
		case GL_PROJECTION: { return (glMatrixState->Top(1)); } break;
		case GL_TEXTURE   : { return (glMatrixState->Top(2)); } break;
		default           : {                  assert(false); } break;
	}

	// fallback
	return (glMatrixState->Top());
}


/*
static bool InDisplayList() {
	GLboolean inListCompile = GL_FALSE;
	glGetBooleanv(GL_LIST_INDEX, &inListCompile);
	return inListCompile;
}
static void CompareMatrices(const CMatrix44f& svm, const CMatrix44f& spm) {
	// calls in display lists will modify the local stack(s)
	// lists need to be completely removed before switching!
	assert(!InDisplayList());

	CMatrix44f cvm;
	CMatrix44f cpm;

	glGetFloatv(GL_MODELVIEW_MATRIX, &cvm.m[0]);
	glGetFloatv(GL_PROJECTION_MATRIX, &cpm.m[0]);

	for (int i = 0; i < 16; i++) {
		assert(epscmp(svm[i], cvm[i], 0.01f));
		assert(epscmp(spm[i], cpm[i], 0.01f));
	}
}
*/

#if 0
void GL::PushMatrix() { glMatrixState->Push(glMatrixState->Top()); }
void GL::PopMatrix() { glMatrixState->Pop(); }

void GL::MultMatrix(const CMatrix44f& m) { glMatrixState->Mult(m); }
void GL::LoadMatrix(const CMatrix44f& m) { glMatrixState->Load(m); }
void GL::LoadIdentity() { LoadMatrix(CMatrix44f::Identity()); }

void GL::Translate(const float3& v) { glMatrixState->Translate(v); }
void GL::Translate(float x, float y, float z) { glMatrixState->Translate({x, y, z}); }
void GL::Scale(const float3& s) { glMatrixState->Scale(s); }
void GL::Scale(float x, float y, float z) { glMatrixState->Scale({x, y, z}); }
void GL::Rotate(float a, float x, float y, float z) { glMatrixState->Rotate(a, x, y, z); }
void GL::RotateX(float a) { glMatrixState->RotateX(a); }
void GL::RotateY(float a) { glMatrixState->RotateY(a); }
void GL::RotateZ(float a) { glMatrixState->RotateZ(a); }

#elif 0
// sanity-checked versions
void GL::PushMatrix() { glMatrixState->Push(glMatrixState->Top());  glPushMatrix(); CompareMatrices(glMatrixState->Top(0), glMatrixState->Top(1)); }
void GL::PopMatrix() { glMatrixState->Pop();  glPopMatrix(); CompareMatrices(glMatrixState->Top(0), glMatrixState->Top(1)); }

void GL::MultMatrix(const CMatrix44f& m) { glMatrixState->Mult(m);  glMultMatrixf(m); CompareMatrices(glMatrixState->Top(0), glMatrixState->Top(1)); }
void GL::LoadMatrix(const CMatrix44f& m) { glMatrixState->Load(m);  glLoadMatrixf(m); CompareMatrices(glMatrixState->Top(0), glMatrixState->Top(1)); }
void GL::LoadIdentity() { LoadMatrix(CMatrix44f::Identity());  glLoadIdentity(); CompareMatrices(glMatrixState->Top(0), glMatrixState->Top(1)); }

void GL::Translate(const float3& v) { glMatrixState->Translate(v);  glTranslatef(v.x, v.y, v.z); CompareMatrices(glMatrixState->Top(0), glMatrixState->Top(1)); }
void GL::Translate(float x, float y, float z) { glMatrixState->Translate({x, y, z});  glTranslatef(x, y, z); CompareMatrices(glMatrixState->Top(0), glMatrixState->Top(1)); }
void GL::Scale(const float3& s) { glMatrixState->Scale(s);  glScalef(s.x, s.y, s.z); CompareMatrices(glMatrixState->Top(0), glMatrixState->Top(1)); }
void GL::Scale(float x, float y, float z) { glMatrixState->Scale({x, y, z});  glScalef(x, y, z); CompareMatrices(glMatrixState->Top(0), glMatrixState->Top(1)); }
void GL::Rotate(float a, float x, float y, float z) { glMatrixState->Rotate(a, x, y, z);  glRotatef(a, x, y, z); CompareMatrices(glMatrixState->Top(0), glMatrixState->Top(1)); } // FIXME: broken
void GL::RotateX(float a) { glMatrixState->RotateX(a);  glRotatef(a, 1.0f, 0.0f, 0.0f); CompareMatrices(glMatrixState->Top(0), glMatrixState->Top(1)); }
void GL::RotateY(float a) { glMatrixState->RotateY(a);  glRotatef(a, 0.0f, 1.0f, 0.0f); CompareMatrices(glMatrixState->Top(0), glMatrixState->Top(1)); }
void GL::RotateZ(float a) { glMatrixState->RotateZ(a);  glRotatef(a, 0.0f, 0.0f, 1.0f); CompareMatrices(glMatrixState->Top(0), glMatrixState->Top(1)); }

#else

void GL::PushMatrix() { glPushMatrix(); }
void GL::PopMatrix() { glPopMatrix(); }

void GL::MultMatrix(const CMatrix44f& m) { glMultMatrixf(m); }
void GL::LoadMatrix(const CMatrix44f& m) { glLoadMatrixf(m); }
void GL::LoadIdentity() { glLoadIdentity(); }

void GL::Translate(const float3& v) { glTranslatef(v.x, v.y, v.z); }
void GL::Translate(float x, float y, float z) { glTranslatef(x, y, z); }
void GL::Scale(const float3& s) { glScalef(s.x, s.y, s.z); }
void GL::Scale(float x, float y, float z) { glScalef(x, y, z); }
void GL::Rotate(float a, float x, float y, float z) { glRotatef(a, x, y, z); }
void GL::RotateX(float a) { glRotatef(a, 1.0f, 0.0f, 0.0f); }
void GL::RotateY(float a) { glRotatef(a, 0.0f, 1.0f, 0.0f); }
void GL::RotateZ(float a) { glRotatef(a, 0.0f, 0.0f, 1.0f); }
#endif


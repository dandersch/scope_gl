/*
** scope_gl.h : Scoped Push/Pop macros of common OpenGL calls to avoid state leaks
**
** IDEA:
** Control the opengl state-machine by putting as many calls as possible into push/pop scopes.
** Using these scopes consistently should make state leaks impossible and makes the entire current
** state for a draw call obvious, since all the state can be set/unset right before the call:
**
**   scope_glUseProgram(shader)
**    scope_glBindTexture(GL_TEXTURE_2D, tex)
**     scope_glEnable(GL_BLEND)
**      scope_glBlendFunc(GL_ONE, GL_SRC_ALPHA)
**       ...
**        scope_glClearColor(0,0,0,0)
**   {
**       glDrawArrays(GL_TRIANGLES, 0, 6);
**   }
**
** More push/pop functions to add:
**    glEnableVertexAttribArray/glDisableVertexAttribArray
**    glActiveTexture() : glGetIntegerv(GL_ACTIVE_TEXTURE, &activeTextureUnit);
**    glScissor
**
** Possible improvements:
** - There should be an easy way to turn off restoring of states (since that requires expensively querying via glGetInteger, etc.).
**   Instead, unset the state without restoring the old state (i.e. glUseProgram(0) instead of glUseProgram(old)).
** - Rename all macros from scope_gl_use_program to scope_glUseProgram to mirror the underlying API call. The macro arguments should
**   also be identical to the passed parameters, so that we can convert any scope_... to a simple call to the OpenGL API by removing
**   'scope_' and appending ';'.
** - We could add calls to glError() at the end of every scope
** - Avoid warning about shadowing variables when putting scope macros on the same line
*/

#define scope_glUseProgram(id)                                                   _scope_glUseProgram(id)
#define scope_glBindVertexArray(vao)                                             _scope_glBindVertexArray(vao)
#define scope_glBindTexture(target,texture)                                      _scope_glBindTexture(target,texture)
#define scope_glBindBuffer(target,buffer)                                        _scope_glBindBuffer(target,buffer)
#define scope_glBindArrayBuffer(vbo)                                             _scope_glBindArrayBuffer(vbo)
#define scope_glEnable(enumval)                                                  _scope_glEnable(enumval)
#define scope_glDisable(enumval)                                                 _scope_glDisable(enumval)
#define scope_glBindFramebuffer(target, fbo)                                     _scope_glBindFramebuffer(target, fbo) /* TODO: untested */
#define scope_glFramebufferTexture(target,attachment,textarget,texture,level)    _scope_glFramebufferTexture(target,attachment,textarget,texture,level)
#define scope_glBindRenderbuffer(target,renderbuffer)                            _scope_glBindRenderbuffer(target,renderbuffer)
#define scope_glFramebufferRenderbuffer(fbo,attachment,renderbuffer)             _scope_glFramebufferRenderbuffer(fbo,attachment,renderbuffer)
#define scope_glViewport(x,y,w,h)                                                _scope_glViewport(x,y,w,h)
#define scope_glClearColor(r,g,b,a)                                              _scope_glClearColor(r,g,b,a)
#define scope_glBlendFunc(src,dst)                                               _scope_glBlendFunc(src,dst)
#define scope_glBlendEquation(eq)                                                _scope_glBlendEquation(eq)
#define scope_glCullFace(mode)                                                   _scope_glCullFace(mode)
#define scope_glFrontFace(orient)                                                _scope_glFrontFace(orient)
/* convenience macros */
#define scope_glBindTexture2D(tex_id)                                            _scope_glBindTexture2D(tex_id)
#define scope_glBindFBO(fbo)                                                     _scope_glBindFBO(fbo)
#define scope_glFramebufferTex2D(attachment,tex)                                 _scope_glFramebufferTex2D(attachment,tex)
#define scope_glBindSSBO(ssbo, binding)                                          _scope_glBindSSBO(ssbo, binding)
#define scope_glTex2DParameter(ext,param,val)                                    _scope_glTex2DParameter(ext,param,val)

#ifdef SCOPE_GL_RESTORE_STATE
#define _scope_glUseProgram(id)                                                  _restore_glUseProgram(id)
#define _scope_glBindVertexArray(vao)                                            _restore_glBindVertexArray(vao)
#define _scope_glBindTexture(target,texture)                                     _restore_glBindTexture(target,texture)
#define _scope_glBindBuffer(target,buffer)                                       _restore_glBindBuffer(target,buffer)
#define _scope_glBindArrayBuffer(vbo)                                            _restore_glBindArrayBuffer(vbo)
#define _scope_glEnable(enumval)                                                 _restore_glEnable(enumval)
#define _scope_glDisable(enumval)                                                _restore_glDisable(enumval)
#define _scope_glBindFramebuffer(target, fbo)                                    _restore_glBindFramebuffer(target, fbo)
#define _scope_glFramebufferTexture(target,attachment,textarget,texture,level)   _restore_glFramebufferTexture(target,attachment,textarget,texture,level)
#define _scope_glBindRenderbuffer(target,renderbuffer)                           _restore_glBindRenderbuffer(target,renderbuffer)
#define _scope_glFramebufferRenderbuffer(fbo,attachment,renderbuffer)            _restore_glFramebufferRenderbuffer(fbo,attachment,renderbuffer)
#define _scope_glViewport(x,y,w,h)                                               _restore_glViewport(x,y,w,h)
#define _scope_glClearColor(r,g,b,a)                                             _restore_glClearColor(r,g,b,a)
#define _scope_glBlendFunc(src,dst)                                              _restore_glBlendFunc(src,dst)
#define _scope_glBlendEquation(eq)                                               _restore_glBlendEquation(eq)
#define _scope_glCullFace(mode)                                                  _restore_glCullFace(mode)
#define _scope_glFrontFace(orient)                                               _restore_glFrontFace(orient)
#define _scope_glBindTexture2D(tex_id)                                           _restore_glBindTexture2D(tex_id)
#define _scope_glBindFBO(fbo)                                                    _restore_glBindFBO(fbo)
#define _scope_glFramebufferTex2D(attachment,tex)                                _restore_glFramebufferTex2D(attachment,tex)
#define _scope_glBindSSBO(ssbo, binding)                                         _restore_glBindSSBO(ssbo, binding)
#define _scope_glTex2DParameter(ext,param,val)                                   _restore_glTex2DParameter(ext,param,val)
#else // SCOPE_GL_RESTORE_STATE
#define _scope_glUseProgram(id)                                                  _unset_glUseProgram(id)
#define _scope_glBindVertexArray(vao)                                            _unset_glBindVertexArray(vao)
#define _scope_glBindTexture(target,texture)                                     _unset_glBindTexture(target,texture)
#define _scope_glBindBuffer(target,buffer)                                       _unset_glBindBuffer(target,buffer)
#define _scope_glBindArrayBuffer(vbo)                                            _unset_glBindArrayBuffer(vbo)
#define _scope_glEnable(enumval)                                                 _unset_glEnable(enumval)
#define _scope_glDisable(enumval)                                                _unset_glDisable(enumval)
#define _scope_glBindFramebuffer(target, fbo)                                    _unset_glBindFramebuffer(target, fbo)
#define _scope_glFramebufferTexture(target,attachment,textarget,texture,level)   _unset_glFramebufferTexture(target,attachment,textarget,texture,level)
#define _scope_glBindRenderbuffer(target,renderbuffer)                           _unset_glBindRenderbuffer(target,renderbuffer)
#define _scope_glFramebufferRenderbuffer(fbo,attachment,renderbuffer)            _unset_glFramebufferRenderbuffer(fbo,attachment,renderbuffer)
#define _scope_glViewport(x,y,w,h)                                               _unset_glViewport(x,y,w,h)
#define _scope_glClearColor(r,g,b,a)                                             _unset_glClearColor(r,g,b,a)
#define _scope_glBlendFunc(src,dst)                                              _unset_glBlendFunc(src,dst)
#define _scope_glBlendEquation(eq)                                               _unset_glBlendEquation(eq)
#define _scope_glCullFace(mode)                                                  _unset_glCullFace(mode)
#define _scope_glFrontFace(orient)                                               _unset_glFrontFace(orient)
#define _scope_glBindTexture2D(tex_id)                                           _unset_glBindTexture2D(tex_id)
#define _scope_glBindFBO(fbo)                                                    _unset_glBindFBO(fbo)
#define _scope_glFramebufferTex2D(attachment,tex)                                _unset_glFramebufferTex2D(attachment,tex)
#define _scope_glBindSSBO(ssbo, binding)                                         _unset_glBindSSBO(ssbo, binding)
#define _scope_glTex2DParameter(ext,param,val)                                   _unset_glTex2DParameter(ext,param,val)
#endif // SCOPE_GL_RESTORE_STATE


#define _restore_glUseProgram(id) \
    for (GLint UQ(prog), UQ(i) = (glGetIntegerv(GL_CURRENT_PROGRAM, &UQ(prog)), glUseProgram(id), 0); \
         (UQ(i) == 0); (UQ(i) += 1, glUseProgram(UQ(prog))))
#define _unset_glUseProgram(id) scope_begin_end_var(glUseProgram(id), glUseProgram(0), prog)

#define _restore_glBindVertexArray(vao) \
    for (GLint UQ(old_vao), UQ(i) = (glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &UQ(old_vao)), glBindVertexArray(vao), 0); \
         (UQ(i) == 0); (UQ(i) += 1, glBindVertexArray(UQ(old_vao))))
#define _unset_glBindVertexArray(vao) scope_begin_end_var(glBindVertexArray(vao), glBindVertexArray(0), vertarr)

#define _restore_glBindTexture(target,texture) \
    for (GLint UQ(old_tex), UQ(i) = (glGetIntegerv(_scope_gl_map_texture_target_to_binding(target), &UQ(old_tex)), glBindTexture(target, texture), 0); \
         (UQ(i) == 0); (UQ(i) += 1, glBindTexture(target, UQ(old_tex))))
#define _unset_glBindTexture(target,texture) scope_begin_end_var(glBindTexture(target, texture), glBindTexture(target, 0), texbind)

#define _restore_glBindTexture2D(tex_id) \
    for (GLint UQ(old_tex), UQ(i) = (glGetIntegerv(GL_TEXTURE_BINDING_2D, &UQ(old_tex)), glBindTexture(GL_TEXTURE_2D, tex_id), 0); \
         (UQ(i) == 0); (UQ(i) += 1, glBindTexture(GL_TEXTURE_2D, UQ(old_tex))))
#define _unset_glBindTexture2D(tex_id) scope_begin_end_var(glBindTexture(GL_TEXTURE_2D, tex_id), glBindTexture(GL_TEXTURE_2D, 0), texbind)

#define _restore_glBindBuffer(target,buffer) \
    for (GLint UQ(old_buf), UQ(i) = (glGetIntegerv(GL_BUFFER_BINDING, &UQ(old_buf)), glBindBuffer(target, vbo), 0); \
         (UQ(i) == 0); (UQ(i) += 1, glBindBuffer(target, UQ(old_vbo))))
#define _unset_glBindBuffer(target,buffer) scope_begin_end_var(glBindBuffer(target, buffer), glBindBuffer(target, 0), glbuf)

#define _restore_glBindArrayBuffer(vbo) \
    for (GLint UQ(old_vbo), UQ(i) = (glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &UQ(old_vbo)), glBindBuffer(GL_ARRAY_BUFFER, vbo), 0); \
         (UQ(i) == 0); (UQ(i) += 1, glBindBuffer(GL_ARRAY_BUFFER, UQ(old_vbo))))
#define _unset_glBindArrayBuffer(vbo) scope_begin_end_var(glBindBuffer(GL_ARRAY_BUFFER, vbo), glBindBuffer(GL_ARRAY_BUFFER, 0), arrbuf)

#define _restore_glEnable(enumval) \
    for (GLint UQ(old_flag), UQ(i) = (glGetBooleanv(enumval, (GLboolean*) &UQ(old_flag)), glEnable(enumval), 0); \
         (UQ(i) == 0); (UQ(i) += 1, (UQ(old_flag) ? glEnable(enumval) : glDisable(enumval))))
#define _unset_glEnable(enumval) scope_begin_end_var(glEnable(enumval), glDisable(enumval), glenable)

#define _restore_glDisable(enumval) \
    for (GLint UQ(old_flag), UQ(i) = (glGetBooleanv(enumval, (GLboolean*) &UQ(old_flag)), glDisable(enumval), 0); \
         (UQ(i) == 0); (UQ(i) += 1, (UQ(old_flag) ? glEnable(enumval) : glDisable(enumval))))
#define _unset_glDisable(enumval) scope_begin_end_var(glDisable(enumval), glEnable(enumval), glenable)

#define _restore_glBindFramebuffer(target, fbo) \
    for (GLint UQ(old_fbo), UQ(i) = (glGetIntegerv(target, &UQ(old_fbo)), glBindFramebuffer(target, fbo), 0); \
         (UQ(i) == 0); (UQ(i) += 1, glBindFramebuffer(target, UQ(old_fbo))))
#define _unset_glBindFramebuffer(target, fbo) scope_begin_end_var(glBindFramebuffer(target, fbo), glBindFramebuffer(target, 0), old_fbo)

#define _restore_glBindFBO(fbo) \
    for (GLint UQ(old_fbo), UQ(i) = (glGetIntegerv(GL_FRAMEBUFFER_BINDING, &UQ(old_fbo)), glBindFramebuffer(GL_FRAMEBUFFER, fbo), 0); \
         (UQ(i) == 0); (UQ(i) += 1, glBindFramebuffer(GL_FRAMEBUFFER, UQ(old_fbo))))
#define _unset_glBindFBO(fbo) scope_begin_end_var(glBindFramebuffer(GL_FRAMEBUFFER, fbo), glBindFramebuffer(GL_FRAMEBUFFER, 0), old_fbo) // version that does not restore state

/* TODO: untested */
/* TODO: Texture1D,Texture2D,Texture3D,Named..Texture missing */
/* TODO: does not restore */
/* NOTE: does not restore previously attached texture, could maybe be achieved with glGetFramebufferAttachmentParameter
 * see https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGetFramebufferAttachmentParameter.xhtml */
#define _restore_glFramebufferTexture(target,attachment,textarget,texture,level) \
    scope_begin_end_var(glFramebufferTexture2D(target, attachment, textarget, texture, level), \
                        glFramebufferTexture2D(target, attachment, textarget,       0,     0), fbotex)
#define _unset_glFramebufferTexture(target,attachment,textarget,texture,level) \
    scope_begin_end_var(glFramebufferTexture2D(target, attachment, textarget, texture, level), \
                        glFramebufferTexture2D(target, attachment, textarget,       0,     0), fbotex)

/* TODO does not restore */
#define _restore_glBindRenderbuffer(target,renderbuffer)                      \
    scope_begin_end_var(glBindRenderbuffer(target, renderbuffer),          \
                        glBindRenderbuffer(target,            0), renbuf)
#define _unset_glBindRenderbuffer(target,renderbuffer) scope_begin_end_var(glBindRenderbuffer(target, renderbuffer), glBindRenderbuffer(target, 0), renbuf)

/* TODO: untested */
/* NOTE: does not restore previously attached texture, could maybe be achieved with glGetFramebufferAttachmentParameter
 * see https://registry.khronos.org/OpenGL-Refpages/gl4/html/glGetFramebufferAttachmentParameter.xhtml */
// attachment = {GL_COLOR_ATTACHMENT0,GL_DEPTH_ATTACHMENT,GL_STENCIL_ATTACHMENT}
/* TODO does not restore */
#define _restore_glFramebufferRenderbuffer(fbo,attachment,renderbuffer) \
    scope_begin_end_var(glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, renderbuffer), \
                        glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, 0), fborenbuf)
#define _unset_glFramebufferRenderbuffer(fbo,attachment,renderbuffer) \
    scope_begin_end_var(glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, renderbuffer), \
                        glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, 0), fborenbuf)

#define _restore_glViewport(x,y,w,h) \
    for (GLint UQ(view)[4], UQ(i) = (glGetIntegerv(GL_VIEWPORT, UQ(view)), glViewport(x, y, w, h), 0); \
         (UQ(i) == 0); (UQ(i) += 1, glViewport(UQ(view)[0], UQ(view)[1], UQ(view)[2], UQ(view)[3])))
#define _unset_glViewport(x,y,w,h) scope_begin_end_var(glViewport(x,y,w,h), glViewport(0,0,0,0), glview)

#define _restore_glClearColor(r,g,b,a) \
    for (GLfloat UQ(clear)[4], UQ(i) = (glGetFloatv(GL_COLOR_CLEAR_VALUE, UQ(clear)), glClearColor(r,g,b,a), 0); \
         (UQ(i) == 0); (UQ(i) += 1, glClearColor(UQ(clear)[0], UQ(clear)[1], UQ(clear)[2], UQ(clear)[3])))
#define _unset_glClearColor(r,g,b,a) scope_begin_end_var(glClearColor(r,g,b,a), glClearColor(0,0,0,0), glclear)

#define _restore_glBlendFunc(src,dst) \
    for (GLint UQ(s), UQ(d), UQ(i) = (glGetIntegerv(GL_BLEND_SRC, &UQ(s)), glGetIntegerv(GL_BLEND_DST, &UQ(d)), glBlendFunc(src, dst), 0); \
         (UQ(i) == 0); (UQ(i) += 1, glBlendFunc(UQ(s), UQ(d))))
#define _unset_glBlendFunc(src,dst) scope_begin_end_var(glBlendFunc(src,dst), glBlendFunc(0,0), glblend)

#define _restore_glBlendEquation(eq) \
    for (GLint UQ(e), UQ(i) = (glGetIntegerv(GL_BLEND_EQUATION, &UQ(e)), glBlendEquation(eq), 0); \
         (UQ(i) == 0); (UQ(i) += 1, glBlendEquation(UQ(e))))
#define _unset_glBlendEquation(eq) scope_begin_end_var(glBlendEquation(eq), glBlendEquation(0), glblendeq)

/* TODO: untested */
#define _restore_glCullFace(mode) \
    for (GLint UQ(m), UQ(i) = (glGetIntegerv(GL_CULL_FACE_MODE, &UQ(m)), glCullFace(mode), 0); \
         (UQ(i) == 0); (UQ(i) += 1, glCullFace(UQ(m))))
#define _unset_glCullFace(mode) scope_begin_end_var(glCullFace(mode), glCullFace(0), glcull)

/* TODO: untested */
#define _restore_glFrontFace(orient) \
    for (GLint UQ(fo), UQ(i) = (glGetIntegerv(GL_FRONT_FACE, &UQ(fo)), glFrontFace(orient), 0); \
         (UQ(i) == 0); (UQ(i) += 1, glFrontFace(UQ(fo))))
#define _unset_glFrontFace(orient) scope_begin_end_var(glFrontface(orient), glFrontFace(0), glfront)

/* TODO: untested */
#define _restore_glBindSSBO(ssbo, binding) \
    for (GLint UQ(old_ssbo), UQ(i) = (glGetIntegerv(GL_SHADER_STORAGE_BUFFER_BINDING, &UQ(old_ssbo)), glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo), glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, ssbo), 0); \
         (UQ(i) == 0); (UQ(i) += 1, glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, UQ(old_ssbo)), glBindBuffer(GL_SHADER_STORAGE_BUFFER, UQ(old_ssbo))))
#define _unset_glBindSSBO(ssbo, binding) scope_begin_end_var((glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo), glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, ssbo)), (glBindBufferBase(GL_SHADER_STORAGE_BUFFER, binding, 0), glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0)), ssbo)

// ext = {i,f,fv,iv,Iiv,Iuiv}
/* TODO: use cross-platform typeof() */
#define _restore_glTex2DParameter(ext,param,val) \
    for (typeof(val) UQ(t2d), UQ(i) = (glGetTexParameter##ext##v(GL_TEXTURE_2D, param, &UQ(t2d)), glTexParameter##ext(GL_TEXTURE_2D, param, val), 0); \
         (UQ(i) == 0); (UQ(i) += 1, glTexParameter##ext(GL_TEXTURE_2D, param, UQ(t2d))))
#define _unset_glTex2DParameter(ext,param,val) scope_begin_end_var(glTexParameter##ext(GL_TEXTURE_2D, param, val), glTexParameter##ext(GL_TEXTURE_2D, param, 0), gltexparam)

/* for pushing and popping uniform values */
/* NOTE: these are extremely wasteful */
#define scope_glUniformMatrix4fv(matrix,name) \
    for (GLint UQ(prog), UQ(j) = (glGetIntegerv(GL_CURRENT_PROGRAM, &UQ(prog)),0); (UQ(j) == 0); UQ(j) += 1) \
    for (GLfloat UQ(mat)[16], UQ(i) = (glGetUniformfv(UQ(prog), glGetUniformLocation(UQ(prog), name), UQ(mat)), glUniformMatrix4fv(glGetUniformLocation(UQ(prog), name), 1, GL_FALSE, matrix), 0); \
         (UQ(i) == 0); (UQ(i) += 1, glUniformMatrix4fv(glGetUniformLocation(UQ(prog), name), 1, GL_FALSE, UQ(mat))))
#define scope_glUniformfv(val,name) \
    for (GLint UQ(prog), UQ(j) = (glGetIntegerv(GL_CURRENT_PROGRAM, &UQ(prog)),0); (UQ(j) == 0); UQ(j) += 1) \
    for (GLfloat UQ(old_val), UQ(i) = (glGetUniformfv(UQ(prog), glGetUniformLocation(UQ(prog), name), &UQ(old_val)), glUniform1f(glGetUniformLocation(UQ(prog), name), val), 0); \
         (UQ(i) == 0); (UQ(i) += 1, glUniform1f(glGetUniformLocation(UQ(prog), name), UQ(old_val))))

/* helper macros */
#define TOKEN_PASTE(a, b) a##b
#define CONCAT(a,b) TOKEN_PASTE(a,b)
#define UQ(name) CONCAT(name, __LINE__) /* unique identifier */

#define scope_begin_end_var(begin, end, var) \
    for (int UQ(var) = (begin, 0); (UQ(var) == 0); (UQ(var) += 1), end)

static inline GLuint _scope_gl_map_texture_target_to_binding(GLuint target) {
    switch (target) {
        case GL_TEXTURE_1D                   : return GL_TEXTURE_BINDING_1D                   ;
        case GL_TEXTURE_2D                   : return GL_TEXTURE_BINDING_2D                   ;
        case GL_TEXTURE_3D                   : return GL_TEXTURE_BINDING_3D                   ;
        case GL_TEXTURE_1D_ARRAY             : return GL_TEXTURE_BINDING_1D_ARRAY             ;
        case GL_TEXTURE_2D_ARRAY             : return GL_TEXTURE_BINDING_2D_ARRAY             ;
        case GL_TEXTURE_RECTANGLE            : return GL_TEXTURE_BINDING_RECTANGLE            ;
        case GL_TEXTURE_CUBE_MAP             : return GL_TEXTURE_BINDING_CUBE_MAP             ;
        case GL_TEXTURE_CUBE_MAP_ARRAY       : return GL_TEXTURE_BINDING_CUBE_MAP_ARRAY       ;
        case GL_TEXTURE_BUFFER               : return GL_TEXTURE_BINDING_BUFFER               ;
        case GL_TEXTURE_2D_MULTISAMPLE       : return GL_TEXTURE_BINDING_2D_MULTISAMPLE       ;
        case GL_TEXTURE_2D_MULTISAMPLE_ARRAY : return GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY ;
        default: return 0;
    }
}

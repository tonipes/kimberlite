// ============================================================================
//  Kimberlite
//
//  Copyright 2020 Toni Pesola. All Rights Reserved.
// ============================================================================

#include <kbextra/font.h>

#include <kb/core.h>
#include <kb/rwops.h>
#include <kb/alloc.h>
#include <kb/renderer.h>
#include <kb/rwops.h>

KB_API void kb_font_data_read(kb_font_data* font, kb_rwops* rwops) {
  KB_ASSERT_NOT_NULL(font);
  KB_ASSERT_NOT_NULL(rwops);

  if (!kb_rwops_check_magic(rwops, KB_CONFIG_FILE_MAGIC_FONT)) {
    kb_printf("Did not find correct magic number!\n");
    return;
  }

  kb_read(rwops, font->info.ascent);
  kb_read(rwops, font->info.descent);
  kb_read(rwops, font->info.line_gap);
  kb_read(rwops, font->info.scale_factor);
  kb_read(rwops, font->info.pixel_height);
  kb_read(rwops, font->info.char_count);
  
  font->info.chars = KB_DEFAULT_ALLOC_TYPE(kb_font_char, font->info.char_count);
  for (uint32_t i = 0; i < font->info.char_count; i++) {
    kb_read(rwops, font->info.chars[i]);
  }

  kb_read(rwops, font->atlas_bitmap_width);
  kb_read(rwops, font->atlas_bitmap_height);
  kb_read(rwops, font->atlas_bitmap_size);

  font->atlas_bitmap = KB_DEFAULT_ALLOC(font->atlas_bitmap_size);
  kb_rwops_read(rwops, font->atlas_bitmap, font->atlas_bitmap_size);

  // Build char table
  kb_table_create(&font->info.char_table, font->info.char_count);
  
  for (uint32_t i = 0; i < font->info.char_count; i++) {
    kb_table_insert(&font->info.char_table, font->info.chars[i].codepoint, i);
  }
}

KB_API void kb_font_data_write(const kb_font_data* font, kb_rwops* rwops) {
  KB_ASSERT_NOT_NULL(font);
  KB_ASSERT_NOT_NULL(rwops);

  kb_write(rwops, KB_CONFIG_FILE_MAGIC_FONT);

  // kb_write(rwops, font->info);
  kb_write(rwops, font->info.ascent);
  kb_write(rwops, font->info.descent);
  kb_write(rwops, font->info.line_gap);
  kb_write(rwops, font->info.scale_factor);
  kb_write(rwops, font->info.pixel_height);
  kb_write(rwops, font->info.char_count);

  for (uint32_t i = 0; i < font->info.char_count; i++) {
    kb_write(rwops, font->info.chars[i]);
  }

  kb_write(rwops, font->atlas_bitmap_width);
  kb_write(rwops, font->atlas_bitmap_height);
  kb_write(rwops, font->atlas_bitmap_size);

  kb_rwops_write(rwops, font->atlas_bitmap, font->atlas_bitmap_size);
}

KB_API void kb_font_data_dump_info(const kb_font_data* font) {
  kb_printf("Font: \n");

  kb_printf("\tChars: %u\n", font->info.char_count);
  
  for (uint32_t i = 0; i < font->info.char_count; ++i) {
    kb_font_char* fc = &font->info.chars[i];

    kb_printf("\t\tChar (%c) adv %i, UV [(%f %f) - (%f %f)]\n",
      fc->codepoint, fc->advance,
      font->info.chars[i].uv_rect_from.x,  font->info.chars[i].uv_rect_from.y,
      font->info.chars[i].uv_rect_to.x,    font->info.chars[i].uv_rect_to.y
    );
    kb_printf("\t\t\tfrom: (%i, %i), to: (%i, %i) lsb %i\n",
      fc->bbox_from.x,  fc->bbox_from.y, 
      fc->bbox_to.x,    fc->bbox_to.y,
      fc->left_bearing
    );
  }

  kb_printf("\tAtlas: (%u %u) of size %u\n", font->atlas_bitmap_width, font->atlas_bitmap_height, font->atlas_bitmap_size);
}

KB_API void kb_font_data_destroy(kb_font_data* font) {
  KB_ASSERT_NOT_NULL(font);

  KB_DEFAULT_FREE(font->atlas_bitmap);
  KB_DEFAULT_FREE(font->info.chars);
  kb_table_destroy(&font->info.char_table);
}

void kb_font_break_line(kb_font_info* info, float* current_y) {
  KB_ASSERT_NOT_NULL(info);
  KB_ASSERT_NOT_NULL(current_y);

  *current_y += (info->ascent - info->descent + info->line_gap) * info->scale_factor;
}

void kb_font_quad_advance(kb_font_info* info, int codepoint, float* current_x, float* current_y, FloatRect* pos, FloatRect* uv) {  
  KB_ASSERT_NOT_NULL(info);
  KB_ASSERT_NOT_NULL(current_x);
  KB_ASSERT_NOT_NULL(current_y);

  uint32_t char_idx = kb_table_get(&info->char_table, codepoint);

  if (char_idx == UINT32_MAX) {
    kb_log_warn("Invalid codepoint '{}'", codepoint);
    return;
  }

  const kb_font_char* fc = &info->chars[char_idx];
  
  if (pos != NULL) {
    pos->from.x  = *current_x + fc->bbox_from.x;
    pos->to.x    = *current_x + fc->bbox_to.x;
    pos->from.y  = *current_y + fc->bbox_from.y;
    pos->to.y    = *current_y + fc->bbox_to.y;
  }

  if (uv != NULL) {
    uv->from.x  = fc->uv_rect_from.x;
    uv->to.x    = fc->uv_rect_to.x;
    uv->from.y  = fc->uv_rect_from.y;
    uv->to.y    = fc->uv_rect_to.y;
  }

  *current_x += fc->advance;
}

#ifndef KB_TOOL_ONLY

struct kb_font_ref {
  kb_font_info                      info;
  kb_texture                        atlas;
  kb_pipeline                       pipeline;
  uint32_t                          atlas_width;
  uint32_t                          atlas_height;
};

KB_RESOURCE_STORAGE_DEF       (font, kb_font, kb_font_ref, KB_CONFIG_MAX_FONTS);
KB_RESOURCE_ALLOC_FUNC_DEF    (font, kb_font, kb_font_create_info, KB_CONFIG_MAX_FONTS);
KB_RESOURCE_DATA_HASHED_DEF   (font, kb_font);

void kb_font_construct(kb_font handle, const kb_font_create_info info) {
  kb_font_ref& ref = font_ref(handle);

  ref.info            = info.info;
  ref.info.chars      = (kb_font_char*) kb_memdup(info.info.chars, sizeof(kb_font_char) * info.info.char_count);
  kb_table_copy(&ref.info.char_table, &info.info.char_table);

  ref.atlas_height  = info.atlas_height;
  ref.atlas_width   = info.atlas_width;
  ref.atlas         = info.atlas;
  ref.pipeline      = info.pipeline;
}

void kb_font_destruct(kb_font handle) { 
  KB_DEFAULT_FREE(font_ref(handle).info.chars);
  kb_table_destroy(&font_ref(handle).info.char_table);
  kb_texture_destruct(font_ref(handle).atlas);
}

KB_API void kb_encoder_bind_font(kb_encoder encoder, kb_font font) {
  kb_encoder_bind_pipeline(encoder, font_ref(font).pipeline);
  
  kb_uniform_slot atlas_slot = kb_pipeline_get_uniform_slot(
    font_ref(font).pipeline, "font_atlas"_h, KB_SHADER_BINDING_TYPE_TEXTURE, KB_SHADER_STAGE_FRAGMENT
  );
  
  kb_encoder_bind_texture(encoder, atlas_slot, font_ref(font).atlas);
}

void kb_encoder_submit_text(kb_encoder encoder, kb_font font, const char* str, Float2 origin, Float2 scale, uint32_t instance_count) {
  uint32_t len = kb_strlen(str);
  if (len == 0) return;

  kb_font_ref& fnt = font_ref(font);

  uint32_t max_vertices = len * 4;
  uint32_t max_indices  = len * 6;
  
  uint32_t vertex_alloc_size = max_vertices  * sizeof(kb_simple_vertex);
  uint32_t index_alloc_size  = max_indices   * sizeof(uint16_t);

  kb_simple_vertex* vertex_data_buffer = (kb_simple_vertex*)  kb_graphics_transient_alloc(vertex_alloc_size,  16);
  uint16_t*         index_data_buffer  = (uint16_t*)          kb_graphics_transient_alloc(index_alloc_size,   16);

  kb_simple_vertex* vertex_data = vertex_data_buffer;
  uint16_t*         index_data  = index_data_buffer;
  
  uint32_t indices  = 0;
  uint32_t vertices = 0;

  Float2 scale_factor = scale * (1.0f /  fnt.info.pixel_height);

  Float2 current_pos {
    0.0f,
    (float) (fnt.info.ascent) * fnt.info.scale_factor, // NOTE: This moves starting pos below origin
  };

  FloatRect pos;
  FloatRect uv;

  for (uint32_t i = 0; i < len; ++i) {
    if (str[i] == '\n') { // Linebreak
      kb_font_break_line(&fnt.info, &current_pos.y);
      current_pos.x = 0.0f;
      continue;
    }

    if (str[i] == '\t') { // Tab
      kb_font_quad_advance(&fnt.info, str[i], &current_pos.x, &current_pos.y, &pos, &uv);
      continue;
    }

    kb_font_quad_advance(&fnt.info, str[i], &current_pos.x, &current_pos.y, &pos, &uv);

    Float2 from = scale_factor * pos.from;
    Float2 to   = scale_factor * pos.to;

    kb_simple_vertex verts[4] = {
      { { origin.x + from.x,  origin.y - from.y,   0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { uv.from.x , uv.from.y } },
      { { origin.x + from.x,  origin.y - to.y,     0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { uv.from.x , uv.to.y   } },
      { { origin.x + to.x,    origin.y - to.y,     0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { uv.to.x ,   uv.to.y   } },
      { { origin.x + to.x,    origin.y - from.y,   0.0f, 1.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { uv.to.x ,   uv.from.y } },
    };
    
    kb_memcpy(vertex_data, verts, sizeof(verts));
    
    index_data[0] = uint16_t(vertices + 0);
    index_data[1] = uint16_t(vertices + 1);
    index_data[2] = uint16_t(vertices + 2);
    index_data[3] = uint16_t(vertices + 0);
    index_data[4] = uint16_t(vertices + 2);
    index_data[5] = uint16_t(vertices + 3);

    vertex_data += 4;
    vertices    += 4;

    index_data  += 6;
    indices     += 6;
  }

  uint32_t vertex_data_buffer_offset  = kb_graphics_transient_offset(vertex_data_buffer);
  uint32_t index_data_buffer_offset   = kb_graphics_transient_offset(index_data_buffer);
  
  kb_encoder_push(encoder);
  
  kb_encoder_bind_buffer(encoder, 0, {}, vertex_data_buffer_offset);
  kb_encoder_bind_index_buffer(encoder, {}, index_data_buffer_offset, KB_INDEX_TYPE_16); 
  kb_encoder_submit(encoder, 0, 0, indices, instance_count);

  kb_encoder_pop(encoder);
}

#endif

/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef __GIMP_DISPLAY_SHELL_SCROLL_H__
#define __GIMP_DISPLAY_SHELL_SCROLL_H__


void   gimp_display_shell_scroll_center_image_coordinate    (GimpDisplayShell       *shell,
                                                             gdouble                 image_x,
                                                             gdouble                 image_y);
void   gimp_display_shell_scroll                            (GimpDisplayShell       *shell,
                                                             gint                    x_offset,
                                                             gint                    y_offset);
void   gimp_display_shell_scroll_set_offset                 (GimpDisplayShell       *shell,
                                                             gint                    offset_x,
                                                             gint                    offset_y);
void   gimp_display_shell_scroll_clamp_offsets              (GimpDisplayShell       *shell);
void   gimp_display_shell_scroll_clamp_and_update           (GimpDisplayShell       *shell);
void   gimp_display_shell_scroll_unoverscrollify            (GimpDisplayShell       *shell,
                                                             gint                    in_offset_x,
                                                             gint                    in_offset_y,
                                                             gint                   *out_offset_x,
                                                             gint                   *out_offset_y);
void   gimp_display_shell_scroll_center_image               (GimpDisplayShell       *shell,
                                                             gboolean                horizontally,
                                                             gboolean                vertically);
void   gimp_display_shell_scroll_center_image_on_next_size_allocate
                                                            (GimpDisplayShell       *shell,
                                                             gboolean                horizontally,
                                                             gboolean                vertically);
void   gimp_display_shell_scroll_get_scaled_viewport        (const GimpDisplayShell *shell,
                                                             gint                   *x,
                                                             gint                   *y,
                                                             gint                   *w,
                                                             gint                   *h);
void   gimp_display_shell_scroll_get_viewport               (const GimpDisplayShell *shell,
                                                             gdouble                *x,
                                                             gdouble                *y,
                                                             gdouble                *w,
                                                             gdouble                *h);
void   gimp_display_shell_scroll_get_disp_offset            (const GimpDisplayShell *shell,
                                                             gint                   *disp_xoffset,
                                                             gint                   *disp_yoffset);
void   gimp_display_shell_scroll_get_render_start_offset    (const GimpDisplayShell *shell,
                                                             gint                   *offset_x,
                                                             gint                   *offset_y);
void   gimp_display_shell_scroll_setup_hscrollbar           (GimpDisplayShell       *shell,
                                                             gdouble                 value);
void   gimp_display_shell_scroll_setup_vscrollbar           (GimpDisplayShell       *shell,
                                                             gdouble                 value);


#endif  /*  __GIMP_DISPLAY_SHELL_SCROLL_H__  */

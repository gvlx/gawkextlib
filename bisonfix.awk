# bisonfix.awk --- tweak awkgram.c for stupid compilers.

# Copyright (C) 2005 the Free Software Foundation, Inc.
# 
# This file is part of GAWK, the GNU implementation of the
# AWK Programming Language.
# 
# GAWK is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
# 
# GAWK is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA

/^#if \(! defined \(yyoverflow\) \\/ {
	line = $0
	sub(/\\$/, "", line)
	getline line2
	sub(/\\$/, "", line2)
	getline line3

	line = line line2 line3
	print line
	next
}

{ print }

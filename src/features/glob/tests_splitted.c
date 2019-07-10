/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tests_splitted.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vtarasiu <vtarasiu@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/07/10 13:29:20 by vtarasiu          #+#    #+#             */
/*   Updated: 2019/07/10 14:29:09 by vtarasiu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "glob_tests.h"

void	range2(void)
{
	range_test_match('b', "[b", false);
	range_test_match(']', "[a\\]]", true);
	range_test_match('-', "[a-]]", true);
	range_test_match('b', "[!abc]", false);
	range_test_match('d', "[!abc]", true);
	range_test_match('!', "[ab!c]", true);
	range_test_match('!', "[abc!]", true);
	range_test_match('!', "[!abc]", true);
	range_test_match('!', "[\\!abc]", true);
	range_test_match('!', "[!!]", false);
	range_test_match('#', "[!-A]", true);
	printf("\n");
}

void	range(void)
{
	range_test_match('b', "[b]", true);
	range_test_match('b', "[ab]", true);
	range_test_match('b', "[bc]", true);
	range_test_match('b', "[abc]", true);
	range_test_match('b', "[\\b]", true);
	range_test_match('b', "[\\bc]", true);
	range_test_match('b', "[a\\b]", true);
	range_test_match('b', "[a\\bc]", true);
	range_test_match('b', "[a-b]", true);
	range_test_match('b', "[b-c]", true);
	range_test_match('b', "[a-c]", true);
	range_test_match('b', "[a-\\c]", true);
	range_test_match('b', "[\\a-c]", true);
	range_test_match('b', "[]", false);
	range_test_match('[', "[[]", true);
	range_test_match(']', "[]]", true);
	range_test_match(']', "[]a]", true);
}

void	match(void)
{
	glob_match_test("foo", "foo", true);
	glob_match_test("foo/bar", "foo/", true);
	glob_match_test("foo", "foo/bar", true);
	glob_match_test("foo/", "foo", true);
	glob_match_test("foo", "foo/", true);
	glob_match_test("foo", "fooo", false);
	glob_match_test("foo", "fo?", true);
	glob_match_test("foo[]", "foo[]", true);
	glob_match_test("foo", "fo[a-z]", true);
	glob_match_test("foo", "f[a-z]o", true);
	glob_match_test("foo", "fo[a-c]", false);
	glob_match_test("fo[a]", "fo[a]", false);
	glob_match_test("fo[a]", "fo\\[a]", true);
	glob_match_test("foo[bar", "foo[bar", true);
	glob_match_test("foo", "fo\\o", true);
	glob_match_test(".git", ".git", true);
	glob_match_test(".git", "?git", false);
	glob_match_test(".git", "[.]git", false);
	glob_match_test("..git", "..git", true);
	glob_match_test("..git", ".?git", true);
	glob_match_test("..git", ".[.]git", true);
	glob_match_test("..", "..", true);
	glob_match_test("..", "??", false);
	glob_match_test("..", ".?", true);
	glob_match_test("..", "?.", false);
}

void	match2(void)
{
	glob_match_test("..", ".[.]", true);
	glob_match_test("..", "[.][.]", false);
	glob_match_test("..", "[.].", false);
	glob_match_test(".", ".", true);
	glob_match_test(".", "?", false);
	glob_match_test(".", "[.]", false);
	glob_match_test(".git", ".*t", true);
	glob_match_test(".git", ".*i", false);
	glob_match_test(".git", ".*", true);
	glob_match_test(".git", ".git*", true);
	glob_match_test(".git", "*", false);
	glob_match_test(".git", "*git", false);
	glob_match_test(".git", ".*git", true);
	glob_match_test("git", "*", true);
	glob_match_test("git", "*t", true);
	glob_match_test("git", "*i*", true);
	glob_match_test("sobaka.exe", "*.*", true);
	glob_match_test("sobaka.exe", "*u*", false);
	glob_match_test("sobaka.exe", "sobaka.*", true);
	glob_match_test("sobaka.exe", "*.exe", true);
	glob_match_test("..", "*", false);
	glob_match_test("..", ".*", true);
	glob_match_test("..", "*.", false);
	glob_match_test(".", ".*", true);
	glob_match_test(".", "*", false);
}

void	match3(void)
{
	glob_match_test(".", "*.", false);
	glob_match_test("sobaka", "*[a-z]aka", true);
	glob_match_test("sobaka", "*b[a-z]ka", true);
	glob_match_test("sobaka", "s*[a-z]ka", true);
	glob_match_test("sobaka", "*b[a-z]*ka", true);
	glob_match_test("sobaka", "s[a-z]*ka", true);
	glob_match_test("sobaka", "*ba[A-Z]a", false);
	glob_match_test("sobaka", "so*u[a-z]a", false);
	printf("\n");
}

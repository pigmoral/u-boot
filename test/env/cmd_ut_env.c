// SPDX-License-Identifier: GPL-2.0
/*
 * (C) Copyright 2015
 * Joe Hershberger, National Instruments, joe.hershberger@ni.com
 */

#include <command.h>
#include <test/env.h>
#include <test/suites.h>
#include <test/ut.h>

static int env_test_env_cmd(struct unit_test_state *uts)
{
	ut_assertok(run_command("setenv non_default_var1 1", 0));
	ut_assert_console_end();

	ut_assertok(run_command("setenv non_default_var2 2", 0));
	ut_assert_console_end();

	ut_assertok(run_command("env print non_default_var1", 0));
	ut_assert_nextline("non_default_var1=1");
	ut_assert_console_end();

	ut_assertok(run_command("env default non_default_var1", 0));
	ut_assert_nextline("WARNING: 'non_default_var1' not in imported env, deleting it!");
	ut_assert_console_end();

	ut_asserteq(1, run_command("env exists non_default_var1", 0));
	ut_assert_console_end();

	ut_asserteq(0, run_command("env exists non_default_var2", 0));
	ut_assert_console_end();

	ut_assertok(run_command("setenv non_default_var1 3", 0));
	ut_assert_console_end();

	ut_assertok(run_command("env default -k non_default_var1", 0));
	ut_assert_console_end();

	ut_asserteq(0, run_command("env exists non_default_var1", 0));
	ut_assert_console_end();

	ut_asserteq(0, run_command("env exists non_default_var2", 0));
	ut_assert_console_end();

	ut_assertok(run_command("env default -k -a -f", 0));
	ut_assert_nextline("## Resetting to default environment");
	ut_assert_console_end();

	ut_asserteq(0, run_command("env exists non_default_var1", 0));
	ut_assert_console_end();

	ut_asserteq(0, run_command("env exists non_default_var2", 0));
	ut_assert_console_end();

	/*
	 * While the following test of "env default -a" by itself
	 * works, it unfortunately causes an unrelated test case,
	 * env_test_fdt_import(), to fail, because the "from_fdt"
	 * variable would be removed.
	 */
#if 0
	ut_assertok(run_command("env default -a", 0));
	ut_assert_nextline("## Resetting to default environment");
	ut_assert_console_end();

	ut_asserteq(1, run_command("env exists non_default_var1", 0));
	ut_assert_console_end();

	ut_asserteq(1, run_command("env exists non_default_var2", 0));
	ut_assert_console_end();
#endif

	return 0;
}
ENV_TEST(env_test_env_cmd, UTF_CONSOLE);

int do_ut_env(struct cmd_tbl *cmdtp, int flag, int argc, char *const argv[])
{
	struct unit_test *tests = UNIT_TEST_SUITE_START(env_test);
	const int n_ents = UNIT_TEST_SUITE_COUNT(env_test);

	return cmd_ut_category("environment", "env_test_",
			       tests, n_ents, argc, argv);
}

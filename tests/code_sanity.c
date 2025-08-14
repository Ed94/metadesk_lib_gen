// This is test strictly for the granular (non-generated) version of the library to make sure it operates correctly

// #define MD_LINKED_LIST_PURE_MACRO 1
// #define MD_DONT_MAP_ANREA_TO_ALLOCATOR_IMPL 1
#include "metadesk.c"

// This program expects to be run from the build directory (where it would be after being built)
#define path_examples          "../examples"
#define path_intro             path_examples "/intro"
#define path_hello_world_mdesk path_intro    "/hello_world.mdesk"

int main()
{
	MD_Context ctx = {0};
	ctx.os_ctx.enable_large_pages = true;
	md_init(& ctx);

	printf("metadesk: got past init!\n");

	MD_Arena* arena = md_arena_alloc(0);
	MD_String8 hello_world_mdesk = md_os_data_from_file_path(arena, md_str8_lit(path_hello_world_mdesk));

	MD_TokenizeResult lexed  = md_tokenize_from_text    (arena, hello_world_mdesk);
	MD_ParseResult    parsed = md_parse_from_text_tokens(arena, md_str8_lit(path_hello_world_mdesk), hello_world_mdesk, lexed.tokens);

	#define str8_list_iter(list, elem) list.first; elem; elem = elem->next
	MD_String8List debug_list = md_debug_string_list_from_tree(arena, parsed.root);

	printf("Parsed listing:\n");
	for (MD_String8Node* elem = str8_list_iter(debug_list, elem))
	{
		MD_String8 entry = elem->string;
		if (md_str8_find_needle(entry, 0, md_str8_lit("\n"), 0) < entry.size) {
			printf("%*s", (int)entry.size, entry.str);
		}
		else {
			printf("%*s\n", (int)entry.size, entry.str);
		}
	}

	MD_SSIZE what = 54;
	MD_String8 test = md_str8_from(arena, what);

	md_deinit(& ctx);
}

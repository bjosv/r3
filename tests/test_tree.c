#include <stdio.h>
#include <check.h>
#include "str.h"
#include "node.h"
#include "token.h"

START_TEST (test_ltrim_slash)
{
    fail_if( strcmp( ltrim_slash("/blog") , "blog" ) != 0 );
    fail_if( strcmp( ltrim_slash("blog") , "blog" ) != 0 );
}
END_TEST

START_TEST (test_node_construct_uniq)
{
    node * n = rtree_create(10);

    node * child = rtree_create(3);

    // fail_if( rtree_add_child(n, strdup("/add") , child) != NULL );
    // fail_if( rtree_add_child(n, strdup("/add") , child) != NULL );

    rtree_free(n);
}
END_TEST

START_TEST (test_node_find_edge)
{
    node * n = rtree_create(10);

    node * child = rtree_create(3);

    fail_if( rtree_add_child(n, strdup("/add") , child) == FALSE );

    fail_if( node_find_edge(n, "/add") == NULL );
    fail_if( node_find_edge(n, "/bar") != NULL );

    rtree_free(n);
}
END_TEST


START_TEST (test_compile)
{
    token_array *t;
    node * n;
    n = rtree_create(10);



    rtree_insert_pathn(n, "/zoo", strlen("/zoo"), NULL);
    rtree_insert_pathn(n, "/foo", strlen("/foo"), NULL);
    rtree_insert_pathn(n, "/bar", strlen("/bar"), NULL);
    rtree_compile(n);
    fail_if( n->combined_pattern );
    fail_if( NULL == node_find_edge_str(n, "/", strlen("/") ) );

    rtree_insert_pathn(n, "/foo/{id}", strlen("/foo/{id}"), NULL);
    rtree_insert_pathn(n, "/{id}", strlen("/{id}"), NULL);
    rtree_compile(n);
    rtree_dump(n, 0);
    fail_if(n->edges[0]->child->combined_pattern == NULL);

    edge *e = node_find_edge_str(n, "/", strlen("/") );
    fail_if( NULL == e );

    /*
    printf( "%s\n", e->pattern );
    printf( "%s\n", e->child->combined_pattern );
    printf( "%s\n", n->edges[0]->child->combined_pattern);
    printf( "%s\n", n->combined_pattern );
    */
    node *m = rtree_match( e->child , "foo", strlen("foo") );
    fail_if( NULL == m );

    m = rtree_match( n , "/foo", strlen("/foo") );
    fail_if( NULL == m );

    m = rtree_match( n , "/zoo", strlen("/zoo") );
    fail_if( NULL == m );

    m = rtree_match( n , "/bar", strlen("/bar") );
    fail_if( NULL == m );

    m = rtree_match( n , "/xxx", strlen("/xxx") );
    fail_if( NULL == m );

    m = rtree_match( n , "/foo/xxx", strlen("/foo/xxx") );
    fail_if( NULL == m );

    m = rtree_match( n , "/not_found", strlen("/not_found") );
    fail_if( NULL == m ); // should be the node of "/"
    ck_assert_int_eq( m->endpoint , 0 ); // should not be an endpoint

}
END_TEST

START_TEST (test_compile_slug)
{
    /*
    char * pattern;
    pattern = compile_slug("{id}", strlen("{id}"));
    ck_assert_str_eq( pattern, "([^/]+)" );
    free(pattern);

    pattern = compile_slug("/{id}", strlen("/{id}"));
    ck_assert_str_eq( pattern, "/([^/]+)" );
    free(pattern);

    pattern = compile_slug("-{id}", strlen("-{id}"));
    ck_assert_str_eq( pattern, "-([^-]+)" );
    free(pattern);

    pattern = compile_slug("{id}-{title}", strlen("{id}-{title}"));
    ck_assert_str_eq( pattern, "([^/]+)-([^-]+)" );
    free(pattern);


    pattern = compile_slug("{id:[a-z]+}", strlen("{id:[a-z]+}") );
    ck_assert_str_eq( pattern, "([a-z]+)" );
    free(pattern);


    pattern = compile_slug("/path/{id:[a-z]+}", strlen("/path/{id:[a-z]+}") );
    ck_assert_str_eq( pattern, "/path/([a-z]+)" );
    free(pattern);
    */

    /*
    char * p = malloc(sizeof(char) * 10);
    strncat(p, "foo", 3);
    free(p);
    */





}
END_TEST


START_TEST (test_rtree_insert_pathn)
{
    node * n = rtree_create(10);

    // printf("Inserting /foo/bar\n");
    rtree_insert_pathn(n, "/foo/bar", strlen("/foo/bar"), NULL);
    // rtree_dump(n, 0);

    // printf("Inserting /foo/zoo\n");
    rtree_insert_pathn(n, "/foo/zoo", strlen("/foo/zoo"), NULL);
    // rtree_dump(n, 0);

    // printf("Inserting /f/id\n");
    rtree_insert_pathn(n, "/f/id", strlen("/f/id") , NULL);
    // rtree_dump(n, 0);

    // printf("Inserting /post/{id}\n");
    rtree_insert_pathn(n, "/post/{id}", strlen("/post/{id}"), NULL);
    // rtree_dump(n, 0);

    // printf("Inserting /post/{handle}\n");
    rtree_insert_pathn(n, "/post/{handle}", strlen("/post/{handle}"), NULL);
    // rtree_dump(n, 0);

    // printf("Inserting /post/{handle}-{id}\n");
    rtree_insert_pathn(n, "/post/{handle}-{id}", strlen("/post/{handle}-{id}"), NULL);
    rtree_compile(n);
    // rtree_dump(n, 0);



    /*
    fail_if( rtree_lookup(n , "/a/jj/kk"    , strlen("/a/jj/kk") ) == NULL );
    fail_if( rtree_lookup(n , "/a/jj"       , strlen("/a/jj") ) != NULL );
    fail_if( rtree_lookup(n , "/a/jj/kk/ll" , strlen("/a/jj/kk/ll") ) != NULL );
    fail_if( rtree_lookup(n, "/xxxx", strlen("xxxx") ) != NULL );
    */


    // fail_if( node_find_edge(n, "/add") == NULL );
    // fail_if( node_find_edge(n, "/bar") != NULL );

    rtree_free(n);
}
END_TEST




START_TEST (test_route_split)
{
    token_array *t;

    t = split_route_pattern("/blog", strlen("/blog") );
    fail_if( t == NULL );
    token_array_dump(t);
    token_array_free(t);

    t = split_route_pattern("/foo/{id}", strlen("/foo/{id}") );
    fail_if( t == NULL );
    token_array_dump(t);
    fail_if( t->len != 2 );
    token_array_free(t);

    t = split_route_pattern("/foo/bar/{id}", strlen("/foo/bar/{id}") );
    fail_if( t == NULL );
    token_array_dump(t);
    fail_if( t->len != 3 );
    token_array_free(t);

    t = split_route_pattern("/{title}", strlen("/{title}") );
    fail_if( t == NULL );
    token_array_dump(t);
    fail_if( t->len != 1 );
    token_array_free(t);

    t = split_route_pattern("/", strlen("/") );
    fail_if( t == NULL );
    token_array_dump(t);
    fail_if( t->len != 1 );
    token_array_free(t);

}
END_TEST

START_TEST (test_token_array)
{
    token_array * l = token_array_create(3);
    fail_if( l == NULL );

    fail_if( FALSE == token_array_append(l, strdup("abc") ) );
    fail_if( l->len != 1 );

    fail_if( FALSE == token_array_append(l, strdup("foo") ) );
    fail_if( l->len != 2 );

    fail_if( FALSE == token_array_append(l, strdup("bar") ) );
    fail_if( l->len != 3 );

    fail_if( FALSE == token_array_append(l, strdup("zoo") ) );
    fail_if( l->len != 4 );

    fail_if( FALSE == token_array_resize(l, l->cap * 2) );

    token_array_free(l);
}
END_TEST

Suite* r3_suite (void) {
        Suite *suite = suite_create("blah");

        TCase *tcase = tcase_create("testcase");
        tcase_add_test(tcase, test_route_split);
        tcase_add_test(tcase, test_token_array);
        tcase_add_test(tcase, test_ltrim_slash);
        tcase_add_test(tcase, test_node_construct_uniq);
        tcase_add_test(tcase, test_node_find_edge);
        tcase_add_test(tcase, test_rtree_insert_pathn);
        tcase_add_test(tcase, test_compile_slug);
        tcase_add_test(tcase, test_compile);

        suite_add_tcase(suite, tcase);

        return suite;
}

int main (int argc, char *argv[]) {
        int number_failed;
        Suite *suite = r3_suite();
        SRunner *runner = srunner_create(suite);
        srunner_run_all(runner, CK_NORMAL);
        number_failed = srunner_ntests_failed(runner);
        srunner_free(runner);
        return number_failed;
}

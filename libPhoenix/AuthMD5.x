
struct AuthMD5 {
    uint32_t Cmd; /* Set to AUTHMD5 */
    string   Login<>;
    string   Md5Password<>;
};

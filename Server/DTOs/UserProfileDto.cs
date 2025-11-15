namespace Server.DTOs
{
    public class UserProfileDto
    {
        public int Id { get; set; }
        public string Username { get; set; }
        public int Age { get; set; }
        public string City { get; set; }
        public string Bio { get; set; }
        public string Gender { get; set; }
        public string PhotoUrl { get; set; }
        public string Login { get; set; }
        public UserDto User { get; set; }
        public List<PhotoDto> Photos { get; set; }
    }
}

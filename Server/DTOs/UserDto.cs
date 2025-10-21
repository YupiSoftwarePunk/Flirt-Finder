namespace Server.DTOs
{
    public class UserDto
    {
        public int Id { get; set; }
        public string Username { get; set; }
        public DateTime CreatedAt { get; set; }
        public string Bio { get; set; }
        public string Gender { get; set; }
        public DateTime? BirthDate { get; set; }
    }
}

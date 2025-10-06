namespace Server.Models
{
    public class Chat
    {
        public int Id { get; set; }
        public string UserId { get; set; }
        public int? PinnedMessageId { get; set; }
    }
}

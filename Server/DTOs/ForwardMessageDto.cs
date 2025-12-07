namespace Server.DTOs
{
    public class ForwardMessageDto
    {
        public int ReceiverId { get; set; }
        public int SenderId { get; set; }
        public string Content { get; set; }
        public bool IsForwarded { get; set; } = true;
    }
}
